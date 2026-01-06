#include "esp32.h"

// Parking space monitoring variables
static int currentParkingSpaceIndex = 0;
static unsigned long lastParkingSpaceCheck = 0;
const unsigned long PARKING_SPACE_CHECK_INTERVAL = 500;

static bool previousOccupancyState[PARKING_SPACES];
static bool spaceInitialized[PARKING_SPACES];

void initParkingMonitor() {
    Serial.begin(115200);
    delay(200);

    initWiFi();
    connect();   // MQTT

    setupAllUltrasonicSensors();

    for (int i = 0; i < PARKING_SPACES; i++) {
        previousOccupancyState[i] = false;
        spaceInitialized[i] = false;
    }

    Serial.println("🅿️ NodeMCU Parking Sensor Ready");
}

void sendParkingSpaceData(int spaceIndex) {
    if (spaceIndex < 0 || spaceIndex >= PARKING_SPACES) return;

    bool isOccupied = isParkingSpaceOccupied(spaceIndex);

    if (!spaceInitialized[spaceIndex] ||
        previousOccupancyState[spaceIndex] != isOccupied) {

        JSONVar json;
        json["space_number"] = "P" + String(spaceIndex + 1);
        json["is_occupied"] = isOccupied;

        String payload = JSON.stringify(json);
        PublishParkingSpace(payload);

        previousOccupancyState[spaceIndex] = isOccupied;
        spaceInitialized[spaceIndex] = true;

        Serial.println("📤 " + payload);
    }
}

void updateParkingMonitor() {
    client.loop();
    if (!client.connected()) {
        connect();
    }

    if (millis() - lastParkingSpaceCheck >= PARKING_SPACE_CHECK_INTERVAL) {
        sendParkingSpaceData(currentParkingSpaceIndex);
        currentParkingSpaceIndex =
            (currentParkingSpaceIndex + 1) % PARKING_SPACES;
        lastParkingSpaceCheck = millis();
    }
}
