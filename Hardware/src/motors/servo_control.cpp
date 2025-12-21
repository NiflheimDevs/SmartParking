#include "motors/servo_control.h"
#include "config.h"

Servo myServo;  // Legacy support
Servo entryServo;
Servo exitServo;

// Legacy function (keeping for compatibility)
void setupServo(int pin) {
    myServo.attach(pin);
}

void setServoAngle(int angle) {
    myServo.write(angle);
}

// Separate entry and exit servo functions
void setupEntryServo(int pin) {
    entryServo.attach(pin);
    closeEntryGate();  // Initialize to closed position
}

void setupExitServo(int pin) {
    exitServo.attach(pin);
    closeExitGate();  // Initialize to closed position
}

void setEntryServoAngle(int angle) {
    entryServo.write(angle);
}

void setExitServoAngle(int angle) {
    exitServo.write(angle);
}

void openEntryGate() {
    setEntryServoAngle(SERVO_OPEN_ANGLE);
}

void closeEntryGate() {
    setEntryServoAngle(SERVO_CLOSED_ANGLE);
}

void openExitGate() {
    setExitServoAngle(SERVO_OPEN_ANGLE);
}

void closeExitGate() {
    setExitServoAngle(SERVO_CLOSED_ANGLE);
}