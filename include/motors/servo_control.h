#ifndef SERVO_CONTROL_H

#define SERVO_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>

void setupServo(int pin);
void setServoAngle(int angle);

#endif