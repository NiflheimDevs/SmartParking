#include "MQTT/mqtt_handler.h"

void handleResponses(String topic, JSONVar response) {
    if (topic == ENTRANCE_RESPONSE_TOPIC) {
        handleEntranceResponse(response);
    } 
    else if (topic == EXIT_RESPONSE_TOPIC) {
        handleExitResponse(response);
    }
    else if (topic == GATE_CONTROL_TOPIC) {
        handleGateControl(response);
    }
    else {
        Serial.println("❌ Unknown topic in response handler: " + topic);
    }
}

void handleEntranceResponse(JSONVar response) {
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

void handleExitResponse(JSONVar response) {
    String price = (const char*)response["price"];
    String rfid = (const char*)response["rfid"];
    String error = (const char*)response["error"];
    
    Serial.println("🚪 Exit Response:");
    Serial.println("  RFID: " + rfid);

    
    if (error == "") {
        Serial.println("✅ RFID authorized - Opening exit gate");
        openExitGate();
    } else {
        Serial.println("❌ RFID not authorized: " + error);
        // Gate remains closed
        closeExitGate();
    }
}

void handleGateControl(JSONVar response) {
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