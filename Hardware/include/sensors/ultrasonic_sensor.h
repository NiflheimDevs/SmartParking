#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "config.h"

void setupAllUltrasonicSensors();
float getDistance(int sensorIndex);
bool isParkingSpaceOccupied(int spaceIndex);

#endif