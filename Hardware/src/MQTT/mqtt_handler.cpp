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
    else if (topic == SPACE_TOPIC) {
        handleParkingSpaceResponse(response);
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
        
        // Light the suggested parking spot blue
        if (parkingSpot > 0) {
            int spaceIndex = parkingSpot - 1; // Convert to 0-based index
            Serial.println("💡 Suggesting parking space " + String(parkingSpot) + " (LED " + String(spaceIndex) + ")");
            setParkingSpaceLED(spaceIndex, COLOR_BLUE);
        }
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

void handleParkingSpaceResponse(JSONVar response) {
    // Currently no specific handling for parking space responses
    Serial.println("ℹParking Space Response received");
    String space = (const char*)response["space_number"];
    bool occupied = (bool)response["is_occupied"];
    Serial.println("  Space: " + space);
    Serial.println("  Occupied: " + String(occupied ? "true" : "false"));

    // Update LED status for the parking space
    int spaceIndex = space.substring(1).toInt() - 1;
    updateParkingSpaceStatus(spaceIndex, occupied);
    
    // Update only the changed parking space LED: red if occupied, green if empty
    // uint32_t color = occupied ? COLOR_RED : COLOR_GREEN;
    // setParkingSpaceLED(spaceIndex, color);
    updateParkingSpaceLEDs();
}