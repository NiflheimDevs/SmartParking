#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 7
#define LED_PIN 2
#define LED_BRIGHTNESS 50

#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_YELLOW  0xFFFF00
#define COLOR_PURPLE  0xFF00FF
#define COLOR_CYAN    0x00FFFF
#define COLOR_WHITE   0xFFFFFF
#define COLOR_OFF     0x000000

void setupLEDStrip();
void setAllLEDs(uint32_t color);
void setLED(uint8_t ledIndex, uint32_t color);
void setLEDBrightness(uint8_t brightness);
void clearAllLEDs();
void showLEDs();

void blinkLEDs(uint32_t color, uint16_t delayMs, uint8_t times);
void rainbowCycle(uint16_t delayMs);
void colorWipe(uint32_t color, uint16_t delayMs);
void theaterChase(uint32_t color, uint16_t delayMs);

void showParkingStatus(bool isOccupied);
void showRFIDStatus(bool isAuthorized);
void showSystemStatus(bool isReady);

void showAllParkingSpacesStatus();
void showParkingSpaceStatus(int spaceIndex, bool isOccupied);
void updateParkingSpaceLEDs();
void showAvailableSpacesCount(int availableCount);
void showParkingLotFull();

#endif
