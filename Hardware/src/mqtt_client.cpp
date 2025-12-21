#include "mqtt_client.h"
#include "config.h"
#include "motors/servo_control.h"
#include "Led/Led.h"
#include <Arduino_JSON.h>

// Define the global MQTT client objects
WiFiClient net;
MQTTClient client;

// Gate timer variables for non-blocking gate auto-close
static unsigned long entryGateOpenTime = 0;
static unsigned long exitGateOpenTime = 0;
static bool entryGateOpen = false;
static bool exitGateOpen = false;

void connect() {
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.print("\nconnecting...");
    while (!client.connect("esp32ClientID") )
    {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.println("\nconnected!");
  
    client.subscribe("parking/entrance/response");   
    client.subscribe("parking/exit/response");
    client.subscribe("parking/control");
  
}

bool initWiFi()
{
    WiFi.begin(ssid, pass);

    // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
    // by Arduino. You need to set the IP address directly.
    client.begin("api.smartparking.niflheimdevs.ir",1883, net);
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
    if (topic == "parking/entrance/response") {
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
            entryGateOpen = true;
            entryGateOpenTime = millis();
            showRFIDStatus(true);
        } else {
            Serial.println("❌ RFID not authorized: " + error);
            showRFIDStatus(false);
            // Gate remains closed
            closeEntryGate();
        }
    }
    
    // Handle exit response
    if (topic == "parking/exit/response") {
        bool exists = (bool)response["exist"];
        String rfid = (const char*)response["rfid"];
        String error = (const char*)response["error"];
        
        Serial.println("🚪 Exit Response:");
        Serial.println("  RFID: " + rfid);
        Serial.println("  Exists: " + String(exists ? "true" : "false"));
        
        if (exists) {
            Serial.println("✅ RFID authorized - Opening exit gate");
            openExitGate();
            exitGateOpen = true;
            exitGateOpenTime = millis();
            showRFIDStatus(true);
        } else {
            Serial.println("❌ RFID not authorized: " + error);
            showRFIDStatus(false);
            // Gate remains closed
            closeExitGate();
        }
    }
    
    // Handle control messages (if needed)
    if (topic == "parking/control") {
        Serial.println("🎛️ Control message received");
        // Add control logic here if needed
    }
}

void updateGateTimers() {
    // Auto-close entry gate after duration
    if (entryGateOpen && (millis() - entryGateOpenTime >= GATE_OPEN_DURATION_MS)) {
        closeEntryGate();
        entryGateOpen = false;
        Serial.println("🚪 Entry gate auto-closed");
    }
    
    // Auto-close exit gate after duration
    if (exitGateOpen && (millis() - exitGateOpenTime >= GATE_OPEN_DURATION_MS)) {
        closeExitGate();
        exitGateOpen = false;
        Serial.println("🚪 Exit gate auto-closed");
    }
}
