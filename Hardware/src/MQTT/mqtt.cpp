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
    while (!client.connect(BOARD_NAME) )
    {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.println("\nconnected!");
  
    client.subscribe(ENTRANCE_RESPONSE_TOPIC);   
    client.subscribe(EXIT_RESPONSE_TOPIC);
    client.subscribe(GATE_CONTROL_TOPIC);
    client.subscribe(SPACE_TOPIC);
}

bool initWiFi()
{   
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
    
    handleResponses(topic, response);
}

void PublishRFID(String cardUID,String topic) {
    JSONVar jsonObj;
    jsonObj["rfid"] = cardUID;
    jsonObj["topic"] = topic;  // Include topic in message for forwarding
    String jsonString = JSON.stringify(jsonObj);
    
    client.publish(topic, jsonString.c_str());
    Serial.print("📤 Sent MQTT message: ");
    Serial.println(jsonString);
}

void PublishParkingSpace(String payload) {
    client.publish(SPACE_TOPIC, payload.c_str());
    Serial.print("📤 Sent MQTT message: ");
    Serial.println(payload);
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