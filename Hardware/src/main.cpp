#include <Arduino.h>
#include "config.h"
#include "motors/servo_control.h"
#include "sensors/ultrasonic_sensor.h"
#include "RFID/RFID_reader.h"
#include "Led/Led.h"

void setup() {
    Serial.begin(115200);
    
    // Initialize all system components
    setupServo(SERVO_PIN);
    setupAllUltrasonicSensors();
    setupRFID(RFID_SS_PIN, RFID_RST_PIN);
    setupLEDStrip();
    
    // Show system ready status
    showSystemStatus(true);
    Serial.println("Smart Parking System with " + String(PARKING_SPACES) + " spaces initialized ✅");
}

void loop() {
    String cardUID;

    // Check RFID authorization for gate access
    if (isAuthorizedCard(cardUID)) {
        Serial.println("Gate opening...");
        showRFIDStatus(true);
        setServoAngle(SERVO_OPEN_ANGLE);
        delay(3000);
        setServoAngle(SERVO_CLOSED_ANGLE);
    }

    // Update all parking space LEDs based on sensor readings
    updateParkingSpaceLEDs();
    
    // Check if parking lot is full
    if (areAllParkingSpacesOccupied()) {
        Serial.println("🚨 All parking spaces are occupied!");
    }

    delay(SENSOR_READ_INTERVAL);
}