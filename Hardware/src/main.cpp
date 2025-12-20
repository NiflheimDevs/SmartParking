#include <Arduino.h>
#include "config.h"
#include "motors/servo_control.h"
#include "sensors/ultrasonic_sensor.h"
#include "RFID/RFID_reader.h"
#include "Led/Led.h"
#include "mqtt_client.h"


void setup() {
    Serial.begin(115200);
    delay(2000);
    
    initWiFi();
    
    // Initialize all system components
    setupServo(SERVO_EXIT_PIN);
    setupServo(SERVO_ENTRY_PIN);
    setupAllUltrasonicSensors();
    setupDualRFID();  // Initialize dual RFID readers (entry and exit)
    setupLEDStrip();
    
    // Show system ready status
    showSystemStatus(true);
    Serial.println("Smart Parking System with " + String(PARKING_SPACES) + " spaces initialized ✅");
}

void loop() {
    String cardUID;

    // Maintain MQTT connection
    client.loop();
    if (!client.connected()) {
        connect();
    }

    // Read Entry RFID reader and send via MQTT
    if (readRFIDEntry(cardUID)) 
    {
        showRFIDStatus(true);
        JSONVar jsonObj;
        jsonObj["rfid"] = cardUID;
        String jsonString = JSON.stringify(jsonObj);
        client.publish("parking/entrance", jsonString.c_str());
    }

    // Read Exit RFID reader and send via MQTT
    if (readRFIDExit(cardUID)) 
    {
        showRFIDStatus(true);
        JSONVar jsonObj;
        jsonObj["rfid"] = cardUID;
        String jsonString = JSON.stringify(jsonObj);
        client.publish("parking/exit", jsonString.c_str());
    }

    
    updateParkingSpaceLEDs();
    
    if (areAllParkingSpacesOccupied()) 
    {
        Serial.println("🚨 All parking spaces are occupied!");
    }

    delay(SENSOR_READ_INTERVAL);
}