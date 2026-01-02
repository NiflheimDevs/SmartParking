#include "esp32_s3.h"

void setupS3(){
    Serial.begin(115200);
    delay(200);
    
    initWiFi();
    connect();   // MQTT

    setupEntryServo(SERVO_ENTRY_PIN);
    setupExitServo(SERVO_EXIT_PIN);
    setupDualRFID();
    setupLEDStrip();
    setupLCD();

    initLCDDisplayManager();
    
    Serial.println("🚗 ESP32-S3 RFID Controller Ready");
}

void Monitor(){
    String cardUID;
    
    client.loop();
    if (!client.connected()) {
        connect();
    }

    updateGateTimers();

    if (readRFIDEntry(cardUID)) {
        PublishRFID(cardUID, ENTRANCE_REQUEST_TOPIC);
        notifyRFIDActivity();
    }

    if (readRFIDExit(cardUID)) {
        PublishRFID(cardUID, EXIT_REQUEST_TOPIC);
        notifyRFIDActivity();
    }

    // Update LCD display manager (handles idle state transitions)
    updateLCDDisplayManager();

    //updateParkingSpaceLEDs();

    delay(SENSOR_READ_INTERVAL);
}
