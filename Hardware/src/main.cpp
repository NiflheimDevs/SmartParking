#include <Arduino.h>
#include "config.h"
#include "motors/servo_control.h"
#include "sensors/ultrasonic_sensor.h"
#include "RFID/RFID_reader.h"
#include "Led/Led.h"
#include "mqtt_client.h"
#include "parking_monitor.h"
#include <Arduino_JSON.h>


void setup() {
    Serial.begin(115200);
    delay(200);
    
    initWiFi();
    
    // Initialize all system components
    setupEntryServo(SERVO_ENTRY_PIN);
    setupExitServo(SERVO_EXIT_PIN);
   // setupAllUltrasonicSensors();
    setupDualRFID();  // Initialize dual RFID readers (entry and exit)
    //setupLEDStrip();
    initParkingMonitor();
    
    // Show system ready status
    // showSystemStatus(true);
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
        JSONVar jsonObj;
        jsonObj["rfid"] = cardUID;
        String jsonString = JSON.stringify(jsonObj);
        client.publish("parking/entrance", jsonString.c_str());
        Serial.print("Sent MQTT message: ");
        Serial.println(jsonString);    
    }

    // Read Exit RFID reader and send via MQTT
    if (readRFIDExit(cardUID)) 
    {
        JSONVar jsonObj;
        jsonObj["rfid"] = cardUID;
        String jsonString = JSON.stringify(jsonObj);
        client.publish("parking/exit", jsonString.c_str());
        Serial.print("Sent MQTT message: ");
        Serial.println(jsonString);
    }

    // Update parking space monitoring (sends data to MQTT)
   // updateParkingMonitor();
    
    //updateParkingSpaceLEDs();
    
    //if (areAllParkingSpacesOccupied()) 
    //{
      //  Serial.println("🚨 All parking spaces are occupied!");
    //}

    delay(SENSOR_READ_INTERVAL);
}