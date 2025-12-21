#include "parking_monitor.h"
#include "config.h"
#include "sensors/ultrasonic_sensor.h"
#include "mqtt_client.h"
#include <Arduino_JSON.h>

// Parking space monitoring variables
static int currentParkingSpaceIndex = 0;
static unsigned long lastParkingSpaceCheck = 0;
const unsigned long PARKING_SPACE_CHECK_INTERVAL = 500;  // 0.5 seconds per space

// Track previous occupancy state for each parking space
static bool previousOccupancyState[PARKING_SPACES];
static bool spaceInitialized[PARKING_SPACES];

void sendParkingSpaceData(int spaceIndex) {
    if (spaceIndex < 0 || spaceIndex >= PARKING_SPACES) {
        return;
    }
    
    bool isOccupied = isParkingSpaceOccupied(spaceIndex);
    
    // Send data if state has changed OR if this is the first time checking this space
    if (!spaceInitialized[spaceIndex] || previousOccupancyState[spaceIndex] != isOccupied) {
        // Format space number as P1, P2, P3, etc. (1-indexed)
        String spaceNumber = "P" + String(spaceIndex + 1);
        
        JSONVar jsonObj;
        jsonObj["space_number"] = spaceNumber;
        jsonObj["is_occupied"] = isOccupied;
        
        String jsonString = JSON.stringify(jsonObj);
        client.publish("parking/space", jsonString.c_str());
        
        Serial.println("📤 Sent parking space data: " + jsonString);
        
        // Update the previous state and mark as initialized
        previousOccupancyState[spaceIndex] = isOccupied;
        spaceInitialized[spaceIndex] = true;
    }
}

void initParkingMonitor() {
    currentParkingSpaceIndex = 0;
    lastParkingSpaceCheck = 0;
    
    // Initialize all previous states and initialization flags
    for (int i = 0; i < PARKING_SPACES; i++) {
        previousOccupancyState[i] = false;
        spaceInitialized[i] = false;
    }
}

void updateParkingMonitor() {
    // Send parking space data with 0.5 second intervals per space
    if (millis() - lastParkingSpaceCheck >= PARKING_SPACE_CHECK_INTERVAL) {
        sendParkingSpaceData(currentParkingSpaceIndex);
        currentParkingSpaceIndex = (currentParkingSpaceIndex + 1) % PARKING_SPACES;
        lastParkingSpaceCheck = millis();
    }
}

