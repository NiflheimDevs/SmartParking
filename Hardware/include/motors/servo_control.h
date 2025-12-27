#ifndef SERVO_CONTROL_H

#define SERVO_CONTROL_H

#include <ESP32Servo.h>
#include "config.h"

// Separate entry and exit servo functions
void setupEntryServo(int pin);
void setupExitServo(int pin);
void setEntryServoAngle(int angle);
void setExitServoAngle(int angle);
void openEntryGate();
void closeEntryGate();
void openExitGate();
void closeExitGate();

void updateGateTimers();

#endif