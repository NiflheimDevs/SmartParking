#include "mesh/espnow_mesh.h"

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

// Callback when data is sent
void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
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
        
        esp_now_send(mac_addr, (uint8_t*)&response, sizeof(response));
        Serial.print("📤 Sent RSSI response: ");
        Serial.println(rssi);
    }
    
    // Handle data packet (forward to MQTT if we're AP)
    if (myData->msgType == 1 && currentRole == ROLE_AP) {
        Serial.print("📨 Received data via ESP-NOW: ");
        Serial.println(myData->data);
        // Forward to MQTT
        extern void forwardESPNOWDataToMQTT(const char* data);
        forwardESPNOWDataToMQTT(myData->data);
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
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("❌ Error initializing ESP-NOW");
        return;
    }
    
    // Register callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);
    
    // Add peer (broadcast initially)
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, peerMacAddress, 6);
    peerInfo.channel = ESPNOW_CHANNEL;
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("❌ Failed to add peer");
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
    
    // Remove old peer
    esp_now_del_peer(peerMacAddress);
    
    // Add new peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, peerMacAddress, 6);
    peerInfo.channel = ESPNOW_CHANNEL;
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("❌ Failed to update peer");
    } else {
        Serial.print("✅ Peer updated: ");
        for (int i = 0; i < 6; i++) {
            Serial.printf("%02X", peerMacAddress[i]);
            if (i < 5) Serial.print(":");
        }
        Serial.println();
    }
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
    
    isWaitingForRSSIResponse = true;
    rssiRequestTime = millis();
    
    esp_now_send(peerMacAddress, (uint8_t*)&rssiRequest, sizeof(rssiRequest));
    
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
        Serial.println("⏱️ RSSI check timeout - keeping current role");
        isWaitingForRSSIResponse = false;
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
    
    struct_message message;
    message.msgType = 1;
    message.rssi = WiFi.RSSI();
    message.deviceId = deviceId;
    strncpy(message.data, data, sizeof(message.data) - 1);
    message.data[sizeof(message.data) - 1] = '\0';
    
    esp_now_send(peerMacAddress, (uint8_t*)&message, sizeof(message));
    Serial.print("📤 Sent via ESP-NOW: ");
    Serial.println(data);
}

int8_t getRSSI() {
    return WiFi.RSSI();
}

// Function to update mesh (call this in loop)
void updateMesh() {
    if (!meshInitialized) return;
    
    // Check RSSI every 5 minutes
    if (millis() - lastRSSICheck >= RSSI_CHECK_INTERVAL) {
        checkRSSIAndSwitchRole();
    }
}

