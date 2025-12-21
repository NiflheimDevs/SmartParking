#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 7
//#define LED_PIN 2
#define LED_BRIGHTNESS 50

#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_OFF     0x000000

void setupLEDStrip();

// Update all parking space LEDs: red if occupied, green if empty
void updateParkingSpaceLEDs();

// Show RFID status: blue for entry response (authorized), off for unauthorized
void showRFIDStatus(bool isAuthorized);

#endif
