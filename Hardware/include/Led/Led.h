#ifndef LED_H
#define LED_H

#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "sensors/ultrasonic_sensor.h"

void setupLEDStrip();

// Update all parking space LEDs: red if occupied, green if empty
void updateParkingSpaceLEDs();
void updateParkingSpaceStatus(int spaceIndex, bool occupied);

// Set a specific parking space LED to a specific color (for suggestions)
void setParkingSpaceLED(int spaceIndex, uint32_t color);

// Get parking space status
bool getParkingSpaceStatus(int spaceIndex);

#endif
