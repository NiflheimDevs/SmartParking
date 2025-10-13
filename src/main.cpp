#include <Arduino.h>
#include "config.h"
#include "motors/servo_control.h"
#include "sensors/ultrasonic_sensor.h"
#include "RFID/RFID_reader.h"

void setup() {
    Serial.begin(115200);
    setupServo(SERVO_PIN);
    setupUltrasonic(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);
    setupRFID(RFID_SS_PIN, RFID_RST_PIN);
    Serial.println("System initialized ✅");
}

void loop() {
    float distance = getDistance();
    String cardUID;

    // RFID check
    if (isAuthorizedCard(cardUID)) {
        Serial.println("Gate opening...");
        setServoAngle(SERVO_OPEN_ANGLE);
        delay(3000); // keep gate open
        setServoAngle(SERVO_CLOSED_ANGLE);
    }

    // Parking sensor logic
    if (distance < PARKING_THRESHOLD) {
        Serial.println("🚗 Vehicle detected!");
    }

    delay(SENSOR_READ_INTERVAL);
}