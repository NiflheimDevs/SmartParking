#ifndef LED_H
#define LED_H

#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "sensors/ultrasonic_sensor.h"

void setupLEDStrip();

// Update all parking space LEDs: red if occupied, green if empty
void updateParkingSpaceLEDs();

#endif
