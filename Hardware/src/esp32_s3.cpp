#include "esp32_s3.h"
#include "mesh/espnow_mesh.h"

void setupS3(){
    Serial.begin(115200);
    delay(200);

    // Initialize ESP-NOW mesh first
    initESPNOWMesh();
    
    // Wait a bit for mesh to initialize
    delay(1000);
    
    // Initialize WiFi/MQTT only if we're AP
    if (isAPRole()) {
        initWiFi();
        connect();   // MQTT
    }

    setupEntryServo(SERVO_ENTRY_PIN);
    setupExitServo(SERVO_EXIT_PIN);
    setupDualRFID();

    Serial.println("🚗 ESP32-S3 RFID Controller Ready");
}

void Monitor(){
    String cardUID;

    // Update mesh (handles RSSI checks every 5 minutes)
    updateMesh();
    
    // Handle WiFi/MQTT reconnection if we're AP
    if (isAPRole()) {
        client.loop();
        if (!client.connected()) {
            connect();
        }
    }

    updateGateTimers();

    if (readRFIDEntry(cardUID)) {
        PublishRFID(cardUID, ENTRANCE_REQUEST_TOPIC);
    }

    if (readRFIDExit(cardUID)) {
        PublishRFID(cardUID, EXIT_REQUEST_TOPIC);
    }

    delay(SENSOR_READ_INTERVAL);
}
