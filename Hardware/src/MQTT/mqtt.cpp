#include "MQTT/mqtt.h"

// Define the global MQTT client objects
WiFiClient net;
MQTTClient client;

void connect() {
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.print("\nconnecting...");
    while (!client.connect(MQTT_CLIENT_ID) )
    {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.println("\nconnected!");
  
    client.subscribe(ENTRANCE_RESPONSE_TOPIC);   
    client.subscribe(EXIT_RESPONSE_TOPIC);
    client.subscribe(GATE_CONTROL_TOPIC);
  
}

bool initWiFi()
{
    // Only connect to WiFi/MQTT if device is AP
    if (!isAPRole()) {
        Serial.println("📱 Station role - Skipping WiFi/MQTT connection");
        return false;
    }
    
    WiFi.begin(ssid, pass);

    client.begin(MQTT_BROKER,MQTT_PORT, net);
    client.onMessage(messageReceived);  // Set message callback handler
 
    connect();
    return WiFi.status() == WL_CONNECTED && client.connected();
}

void messageReceived(String &topic, String &payload) {
    Serial.println("📨 MQTT Message received:");
    Serial.println("  Topic: " + topic);
    Serial.println("  Payload: " + payload);
    
    // Parse JSON payload
    JSONVar response = JSON.parse(payload);
    
    if (JSON.typeof(response) == "undefined") {
        Serial.println("❌ Failed to parse JSON response");
        return;
    }
    
    // Forward MQTT messages to ESP32-S3 via ESP-NOW when NodeMCU is AP
    // Forward these topics: GATE_CONTROL_TOPIC, ENTRANCE_RESPONSE_TOPIC, EXIT_RESPONSE_TOPIC
    if (isAPRole() && getDeviceId() == 1 && (topic == GATE_CONTROL_TOPIC || topic == ENTRANCE_RESPONSE_TOPIC || topic == EXIT_RESPONSE_TOPIC)) {
        // Create JSON with topic and payload for forwarding
        JSONVar forwardData;
        forwardData["topic"] = topic;
        forwardData["payload"] = payload;
        String forwardString = JSON.stringify(forwardData);
        
        extern void sendDataViaMesh(const char* data);
        sendDataViaMesh(forwardString.c_str());
        Serial.print("📡 Forwarded MQTT message to ESP32-S3 via ESP-NOW: ");
        Serial.println(topic);
    }
    
    handleResponses(topic, response);
}

void PublishRFID(String cardUID,String topic) {
    JSONVar jsonObj;
    jsonObj["rfid"] = cardUID;
    jsonObj["topic"] = topic;  // Include topic in message for forwarding
    String jsonString = JSON.stringify(jsonObj);
    
    if (isAPRole()) {
        // We're AP, publish directly to MQTT
        client.publish(topic, jsonString.c_str());
        Serial.print("📤 Sent MQTT message: ");
        Serial.println(jsonString);
    } else {
        // We're Station (ESP32-S3), send via ESP-NOW to NodeMCU (AP)
        Serial.print("📡 Station role: Sending RFID data via ESP-NOW to AP: ");
        Serial.println(jsonString);
        sendDataViaMesh(jsonString.c_str());
    }
}

void PublishParkingSpace(String payload) {
    if (isAPRole()) {
        // We're AP, publish directly to MQTT
        client.publish(SPACE_TOPIC, payload.c_str());
        Serial.print("📤 Sent MQTT message: ");
        Serial.println(payload);
    } else {
        // We're Station, send via ESP-NOW
        JSONVar jsonObj = JSON.parse(payload);
        jsonObj["topic"] = SPACE_TOPIC;  // Include topic for forwarding
        String jsonString = JSON.stringify(jsonObj);
        sendDataViaMesh(jsonString.c_str());
    }
}

void forwardESPNOWDataToMQTT(const char* data) {
    if (!isAPRole()) return;
    
    // Parse the data to extract topic and payload
    JSONVar jsonObj = JSON.parse(data);
    
    if (JSON.typeof(jsonObj) == "undefined") {
        Serial.println("❌ Failed to parse ESP-NOW data");
        return;
    }
    
    String topic = (const char*)jsonObj["topic"];
    if (topic.length() == 0) {
        Serial.println("❌ No topic in ESP-NOW data");
        return;
    }
    
    // Remove topic from JSON before publishing
    jsonObj["topic"] = nullptr; // Set to null to effectively remove from payload
    String payload = JSON.stringify(jsonObj);
    
    client.publish(topic.c_str(), payload.c_str());
    Serial.print("📤 Forwarded to MQTT [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(payload);
}