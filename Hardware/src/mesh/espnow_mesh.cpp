#include "mesh/espnow_mesh.h"
#include <Arduino_JSON.h>
#include "motors/servo_control.h"

// Global variables
static DeviceRole currentRole = ROLE_STATION;  // Start as station, will be determined by RSSI
static uint8_t peerMacAddress[6] = PEER_MAC_ADDRESS;  // From config.h
static unsigned long lastRSSICheck = 0;
static bool meshInitialized = false;
static uint8_t deviceId = 0;  // 0 = ESP32-S3, 1 = NodeMCU (set during init)
static int8_t lastRSSIValue = -100;
static int8_t ourRSSIValue = -100;
static bool isWaitingForRSSIResponse = false;
static unsigned long rssiRequestTime = 0;
static const unsigned long RSSI_RESPONSE_TIMEOUT = 2000;  // 2 seconds timeout

// ESP-NOW send flow control
static bool isSending = false;
static unsigned long lastSendTime = 0;
static const unsigned long MIN_SEND_INTERVAL = 50;  // Minimum 50ms between sends
static const unsigned long SEND_TIMEOUT = 500;  // 500ms timeout for send completion

// Helper to add/update a peer with sane defaults
static bool addPeer(const uint8_t* mac) {
    esp_now_peer_info_t peerInfo{};
    memcpy(peerInfo.peer_addr, mac, 6);
    // channel 0 follows the current WiFi channel; otherwise fall back to ESPNOW_CHANNEL
    peerInfo.channel = WiFi.isConnected() ? 0 : ESPNOW_CHANNEL;
    peerInfo.encrypt = false;
    peerInfo.ifidx = WIFI_IF_STA;

    esp_now_del_peer(mac);  // OK if it was not present
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("❌ Failed to add peer");
        return false;
    }
    Serial.print("✅ Peer configured: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
    return true;
}

// Callback when data is sent
void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
    isSending = false;  // Mark send as complete
    Serial.print("📤 ESP-NOW Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

// Callback when data is received
void onDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len) {
    struct_message* myData = (struct_message*)incomingData;
    
    Serial.print("📥 ESP-NOW Data Received from: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
    
    // Store peer MAC address when we receive data
    bool isBroadcast = true;
    for (int i = 0; i < 6; i++) {
        if (mac_addr[i] != 0xFF) {
            isBroadcast = false;
            break;
        }
    }
    if (!isBroadcast) {
        memcpy(peerMacAddress, mac_addr, 6);
        addPeer(peerMacAddress);  // lock in unicast peer for future sends
    }
    
    // Handle RSSI check response
    if (myData->msgType == 0) {
        if (isWaitingForRSSIResponse) {
            lastRSSIValue = myData->rssi;
            Serial.print("📶 Received RSSI: ");
            Serial.println(lastRSSIValue);
            isWaitingForRSSIResponse = false;
        }
        
        // Respond with our RSSI (use WiFi.RSSI() which works in station mode)
        struct_message response;
        response.msgType = 0;
        // WiFi.RSSI() returns the RSSI of the last received packet in station mode
        int8_t rssi = WiFi.RSSI();
        if (rssi == 0) rssi = -100;  // Default if not available
        response.rssi = rssi;
        response.deviceId = deviceId;
        ourRSSIValue = rssi;
        
        // Wait for any pending send to complete (non-blocking with timeout)
        unsigned long waitStart = millis();
        while (isSending && (millis() - waitStart < 100)) {  // Shorter timeout for response
            delay(5);
        }
        
        // Ensure minimum interval
        unsigned long timeSinceLastSend = millis() - lastSendTime;
        if (timeSinceLastSend < MIN_SEND_INTERVAL) {
            delay(MIN_SEND_INTERVAL - timeSinceLastSend);
        }
        
        isSending = true;
        lastSendTime = millis();
        
        esp_err_t result = esp_now_send(mac_addr, (uint8_t*)&response, sizeof(response));
        if (result != ESP_OK) {
            isSending = false;
            Serial.println("❌ Failed to send RSSI response");
        } else {
            Serial.print("📤 Sent RSSI response: ");
            Serial.println(rssi);
        }
    }
    
    // Handle data packet
    if (myData->msgType == 1) {
        Serial.print("📨 Received data via ESP-NOW: ");
        Serial.println(myData->data);
        
        if (currentRole == ROLE_AP) {
            // We're AP (NodeMCU), forward ESP-NOW data from ESP32-S3 to MQTT
            Serial.println("📡 AP role: Forwarding ESP-NOW data to MQTT");
            extern void forwardESPNOWDataToMQTT(const char* data);
            forwardESPNOWDataToMQTT(myData->data);
        } else if (currentRole == ROLE_STATION) {
            // We're Station (ESP32-S3), check if this is a forwarded MQTT message
            // Parse the data to check if it contains topic and payload
            JSONVar jsonObj = JSON.parse(myData->data);
            
            if (JSON.typeof(jsonObj) != "undefined") {
                String topic = (const char*)jsonObj["topic"];
                String payload = (const char*)jsonObj["payload"];
                
                // Check if this is one of the topics we need to handle
                if (topic == GATE_CONTROL_TOPIC || topic == ENTRANCE_RESPONSE_TOPIC || topic == EXIT_RESPONSE_TOPIC) {
                    Serial.print("📥 Received forwarded MQTT message: ");
                    Serial.println(topic);
                    
                    // Parse the payload JSON
                    JSONVar response = JSON.parse(payload);
                    
                    if (JSON.typeof(response) != "undefined") {
                        // Handle entrance response
                        if (topic == ENTRANCE_RESPONSE_TOPIC) {
                            bool exists = (bool)response["exist"];
                            String rfid = (const char*)response["rfid"];
                            int parkingSpot = (int)response["parking_spot"];
                            String error = (const char*)response["error"];
                            
                            Serial.println("🚪 Entrance Response (via ESP-NOW):");
                            Serial.println("  RFID: " + rfid);
                            Serial.println("  Exists: " + String(exists ? "true" : "false"));
                            Serial.println("  Parking Spot: " + String(parkingSpot));
                            
                            if (exists) {
                                Serial.println("✅ RFID authorized - Opening entry gate");
                                openEntryGate();
                            } else {
                                Serial.println("❌ RFID not authorized: " + error);
                                closeEntryGate();
                            }
                        }
                        
                        // Handle exit response
                        if (topic == EXIT_RESPONSE_TOPIC) {
                            bool exists = (bool)response["exist"];
                            String rfid = (const char*)response["rfid"];
                            String error = (const char*)response["error"];
                            
                            Serial.println("🚪 Exit Response (via ESP-NOW):");
                            Serial.println("  RFID: " + rfid);
                            Serial.println("  Exists: " + String(exists ? "true" : "false"));
                            
                            if (exists) {
                                Serial.println("✅ RFID authorized - Opening exit gate");
                                openExitGate();
                            } else {
                                Serial.println("❌ RFID not authorized: " + error);
                                closeExitGate();
                            }
                        }
                        
                        // Handle gate control
                        if (topic == GATE_CONTROL_TOPIC) {
                            String gate = (const char*)response["gate"];
                            bool state = (bool)response["state"];
                            
                            Serial.println("🎛️ Gate Control (via ESP-NOW):");
                            Serial.println("  Gate: " + gate);
                            Serial.println("  State: " + String(state ? "open" : "close"));
                            
                            if (gate == "entrance") {
                                if (state) {
                                    Serial.println("✅ Opening entrance gate");
                                    openEntryGate();
                                } else {
                                    Serial.println("🔒 Closing entrance gate");
                                    closeEntryGate();
                                }
                            } else if (gate == "exit") {
                                if (state) {
                                    Serial.println("✅ Opening exit gate");
                                    openExitGate();
                                } else {
                                    Serial.println("🔒 Closing exit gate");
                                    closeExitGate();
                                }
                            }
                        }
                    }
                } else {
                    // Not a forwarded MQTT message, treat as regular data
                    // (This handles the existing data forwarding from Station to AP)
                }
            }
        }
    }
}

void initESPNOWMesh() {
    if (meshInitialized) return;
    
    // Determine device ID based on build flags
    #if DEVICE_TYPE == DEVICE_ESP32_S3
        deviceId = 0;
    #elif DEVICE_TYPE == DEVICE_NODEMCU
        deviceId = 1;
    #endif
    
    // Set device as WiFi Station
    WiFi.mode(WIFI_STA);
    // Ensure we're on the expected ESPNOW channel before any WiFi connection
    esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("❌ Error initializing ESP-NOW");
        return;
    }
    
    // Register callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);
    
    // Add peer (broadcast initially)
    if (!addPeer(peerMacAddress)) {
        return;
    }
    
    meshInitialized = true;
    Serial.println("✅ ESP-NOW Mesh initialized");
    
    // Print device MAC address for configuration
    printDeviceMAC();
    
    // Perform initial RSSI check
    delay(1000);
    checkRSSIAndSwitchRole();
}

void printDeviceMAC() {
    Serial.print("📱 Device MAC Address: ");
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
    Serial.println("💡 Update PEER_MAC_ADDRESS in config.h with the other device's MAC");
}

void setPeerMacAddress(uint8_t* mac) {
    memcpy(peerMacAddress, mac, 6);
    addPeer(peerMacAddress);
}

void checkRSSIAndSwitchRole() {
    if (!meshInitialized) return;
    
    Serial.println("📶 Checking RSSI...");
    
    // Request RSSI from peer
    struct_message rssiRequest;
    rssiRequest.msgType = 0;
    // Get our RSSI (WiFi.RSSI() works in station mode for last received packet)
    int8_t rssi = WiFi.RSSI();
    if (rssi == 0) rssi = -100;  // Default if not available
    rssiRequest.rssi = rssi;
    rssiRequest.deviceId = deviceId;
    ourRSSIValue = rssi;
    
    // Wait for any pending send to complete
    unsigned long waitStart = millis();
    while (isSending && (millis() - waitStart < SEND_TIMEOUT)) {
        delay(10);
    }
    
    // Ensure minimum interval
    unsigned long timeSinceLastSend = millis() - lastSendTime;
    if (timeSinceLastSend < MIN_SEND_INTERVAL) {
        delay(MIN_SEND_INTERVAL - timeSinceLastSend);
    }
    
    isSending = true;
    lastSendTime = millis();
    isWaitingForRSSIResponse = true;
    rssiRequestTime = millis();
    
    esp_err_t result = esp_now_send(peerMacAddress, (uint8_t*)&rssiRequest, sizeof(rssiRequest));
    if (result != ESP_OK) {
        isSending = false;
        Serial.println("❌ Failed to send RSSI request");
    }
    
    // Wait for response (with timeout)
    unsigned long startWait = millis();
    while (isWaitingForRSSIResponse && (millis() - startWait < RSSI_RESPONSE_TIMEOUT)) {
        delay(10);
    }
    
    if (!isWaitingForRSSIResponse) {
        // Got response, compare RSSI
        int8_t ourRSSI = ourRSSIValue;
        int8_t peerRSSI = lastRSSIValue;
        
        Serial.print("📊 Our RSSI: ");
        Serial.print(ourRSSI);
        Serial.print(" | Peer RSSI: ");
        Serial.println(peerRSSI);
        
        // Better RSSI (higher value) becomes AP
        // If RSSI values are similar (within 3 dB), device with lower ID becomes AP
        int8_t rssiDiff = ourRSSI - peerRSSI;
        if (ourRSSI > peerRSSI + 3 || ((rssiDiff >= -3 && rssiDiff <= 3) && deviceId < 1)) {
            if (currentRole != ROLE_AP) {
                Serial.println("🔄 Switching to AP role (better RSSI)");
                switchToAPRole();
            }
        } else {
            if (currentRole != ROLE_STATION) {
                Serial.println("🔄 Switching to Station role");
                switchToStationRole();
            }
        }
    } else {
        Serial.println("⏱️ RSSI check timeout");
        isWaitingForRSSIResponse = false;
        // Ensure at least one device becomes AP so the network comes up
        if (deviceId == 0 && currentRole != ROLE_AP) {
            Serial.println("⚡ Forcing AP role (no peer response)");
            switchToAPRole();
        }
    }
    
    lastRSSICheck = millis();
}

void switchToAPRole() {
    if (currentRole == ROLE_AP) return;  // Already AP
    
    currentRole = ROLE_AP;
    Serial.println("📡 Role: Access Point (AP) - Connecting to WiFi/MQTT");
    
    // Initialize WiFi/MQTT connection
    extern bool initWiFi();
    extern void connect();
    initWiFi();
    connect();
    addPeer(peerMacAddress);  // refresh peer to follow current WiFi channel
}

void switchToStationRole() {
    if (currentRole == ROLE_STATION) return;  // Already Station
    
    currentRole = ROLE_STATION;
    Serial.println("📱 Role: Station - Disconnecting WiFi/MQTT, using ESP-NOW");
    
    // Disconnect WiFi/MQTT if connected
    if (client.connected()) {
        client.disconnect();
    }
    WiFi.disconnect();
}

bool isAPRole() {
    return currentRole == ROLE_AP;
}

DeviceRole getCurrentRole() {
    return currentRole;
}

void sendDataViaMesh(const char* data) {
    if (!meshInitialized) return;
    
    // Wait for previous send to complete (with timeout)
    unsigned long waitStart = millis();
    while (isSending && (millis() - waitStart < SEND_TIMEOUT)) {
        delay(10);
    }
    
    // Ensure minimum interval between sends
    unsigned long timeSinceLastSend = millis() - lastSendTime;
    if (timeSinceLastSend < MIN_SEND_INTERVAL) {
        delay(MIN_SEND_INTERVAL - timeSinceLastSend);
    }
    
    // Mark as sending
    isSending = true;
    lastSendTime = millis();
    
    struct_message message;
    message.msgType = 1;
    message.rssi = WiFi.RSSI();
    message.deviceId = deviceId;
    strncpy(message.data, data, sizeof(message.data) - 1);
    message.data[sizeof(message.data) - 1] = '\0';
    
    esp_err_t result = esp_now_send(peerMacAddress, (uint8_t*)&message, sizeof(message));
    
    if (result != ESP_OK) {
        isSending = false;  // Reset flag on error
        Serial.print("❌ ESP-NOW Send Error: ");
        Serial.println(result);
    } else {
        Serial.print("📤 Queued ESP-NOW send: ");
        Serial.println(data);
    }
}

int8_t getRSSI() {
    return WiFi.RSSI();
}

uint8_t getDeviceId() {
    return deviceId;
}

// Function to update mesh (call this in loop)
void updateMesh() {
    if (!meshInitialized) return;
    
    // Check RSSI every 5 minutes
    if (millis() - lastRSSICheck >= RSSI_CHECK_INTERVAL) {
        checkRSSIAndSwitchRole();
    }
}

