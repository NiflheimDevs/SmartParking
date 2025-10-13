#include "motors/servo_control.h"

Servo myServo;

void setupServo(int pin) {
    myServo.attach(pin);
}

void setServoAngle(int angle) {
    myServo.write(angle);
}