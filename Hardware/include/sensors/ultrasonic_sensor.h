#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

void setupUltrasonic(int trigPin, int echoPin);
float getDistance();

#endif