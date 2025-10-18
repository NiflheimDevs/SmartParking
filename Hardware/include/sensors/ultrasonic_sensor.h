#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>
#include "config.h"

void setupUltrasonic(int trigPin, int echoPin);
float getDistance();

void setupAllUltrasonicSensors();
float getDistance(int sensorIndex);
bool isParkingSpaceOccupied(int spaceIndex);
bool areAllParkingSpacesOccupied();
int getAvailableParkingSpaces();

#endif