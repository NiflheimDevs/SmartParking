#include <Arduino.h>
#include "config.h"
#include "motors/servo_control.h"
#include "sensors/ultrasonic_sensor.h"
#include "RFID/RFID_reader.h"
#include "Led/Led.h"
#include "MQTT/mqtt.h"
#include "parking_monitor.h"


void setup() {
    Serial.begin(115200);
    delay(200);
    
    initWiFi();
    
    // Initialize all system components
    setupEntryServo(SERVO_ENTRY_PIN);
    setupExitServo(SERVO_EXIT_PIN);
   // setupAllUltrasonicSensors();
    setupDualRFID();
    //setupLEDStrip();
    initParkingMonitor();
    
    // Show system ready status
    Serial.println("Smart Parking System with " + String(PARKING_SPACES) + " spaces initialized ✅");
}

void loop() {
    String cardUID;

    // Maintain MQTT connection
    client.loop();
    if (!client.connected()) {
        connect();
    }
    
    // Update gate timers for auto-close functionality
    updateGateTimers();

    // Read Entry RFID reader and send via MQTT
    if (readRFIDEntry(cardUID)) 
    {
        PublishRFID(cardUID,ENTRANCE_REQUEST_TOPIC);   
    }

    // Read Exit RFID reader and send via MQTT
    if (readRFIDExit(cardUID)) 
    {
        PublishRFID(cardUID,EXIT_REQUEST_TOPIC);
    }

    // Update parking space monitoring (sends data to MQTT)
    // updateParkingMonitor();    

    delay(SENSOR_READ_INTERVAL);
}