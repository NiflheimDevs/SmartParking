#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>
#include "config.h"

void setupAllUltrasonicSensors();
float getDistance(int sensorIndex);
bool isParkingSpaceOccupied(int spaceIndex);

#endif