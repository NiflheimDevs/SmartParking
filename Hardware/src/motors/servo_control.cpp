#include "motors/servo_control.h"

Servo entryServo;
Servo exitServo;

// Gate timer variables for non-blocking gate auto-close
static unsigned long entryGateOpenTime = 0;
static unsigned long exitGateOpenTime = 0;
static bool entryGateOpen = false;
static bool exitGateOpen = false;

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
    entryGateOpen = true;
    entryGateOpenTime = millis();
}

void closeEntryGate() {
    setEntryServoAngle(SERVO_CLOSED_ANGLE);
    entryGateOpen = false;
}

void openExitGate() {
    setExitServoAngle(SERVO_CLOSED_ANGLE);
    exitGateOpen = true;
    exitGateOpenTime = millis();
}

void closeExitGate() {
    setExitServoAngle(SERVO_OPEN_ANGLE);
    exitGateOpen = false;
}

void updateGateTimers() {
    // Auto-close entry gate after duration
    if (entryGateOpen && (millis() - entryGateOpenTime >= GATE_OPEN_DURATION_MS)) {
        closeEntryGate();
        Serial.println("🚪 Entry gate auto-closed");
    }
    
    // Auto-close exit gate after duration
    if (exitGateOpen && (millis() - exitGateOpenTime >= GATE_OPEN_DURATION_MS)) {
        closeExitGate();
        Serial.println("🚪 Exit gate auto-closed");
    }
}