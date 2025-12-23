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
    
    // Handle entrance response
    if (topic == ENTRANCE_RESPONSE_TOPIC) {
        bool exists = (bool)response["exist"];
        String rfid = (const char*)response["rfid"];
        int parkingSpot = (int)response["parking_spot"];
        String error = (const char*)response["error"];
        
        Serial.println("🚪 Entrance Response:");
        Serial.println("  RFID: " + rfid);
        Serial.println("  Exists: " + String(exists ? "true" : "false"));
        Serial.println("  Parking Spot: " + String(parkingSpot));
        
        if (exists) {
            Serial.println("✅ RFID authorized - Opening entry gate");
            openEntryGate();
        } else {
            Serial.println("❌ RFID not authorized: " + error);
            // Gate remains closed
            closeEntryGate();
        }
    }
    
    // Handle exit response
    if (topic == EXIT_RESPONSE_TOPIC) {
        bool exists = (bool)response["exist"];
        String rfid = (const char*)response["rfid"];
        String error = (const char*)response["error"];
        
        Serial.println("🚪 Exit Response:");
        Serial.println("  RFID: " + rfid);
        Serial.println("  Exists: " + String(exists ? "true" : "false"));
        
        if (exists) {
            Serial.println("✅ RFID authorized - Opening exit gate");
            openExitGate();
        } else {
            Serial.println("❌ RFID not authorized: " + error);
            // Gate remains closed
            closeExitGate();
        }
    }
    
    // Handle control messages 
    if (topic == GATE_CONTROL_TOPIC) {        
        String gate = (const char*)response["gate"];
        bool state = (bool)response["state"];
        
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
        } 
        else if (gate == "exit") {
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

void PublishRFID(String cardUID,String topic) {
    JSONVar jsonObj;
    jsonObj["rfid"] = cardUID;
    String jsonString = JSON.stringify(jsonObj);
    client.publish(topic, jsonString.c_str());
    Serial.print("Sent MQTT message: ");
    Serial.println(jsonString);
}