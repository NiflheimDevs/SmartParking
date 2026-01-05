#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"

void setupLCD();

void printCenter(String text);
void printCenterAtY(String text, int16_t yPos);


// Display functions for different screens
void showStartupScreen();
void showEntranceAuthorized(int parkingSpot);
void showEntranceUnauthorized();
void showExitAuthorized(String price);
void showExitUnauthorized();
void clearScreen();

// RGB565 color constants (16-bit color format: RRRRR GGGGGG BBBBB)
#define BLACK       0x0000  // (0, 0, 0)
#define WHITE       0xFFFF  // (255, 255, 255)
#define RED         0xF800  // (255, 0, 0)
#define GREEN       0x07E0  // (0, 255, 0)
#define BLUE        0x001F  // (0, 0, 255)
#define YELLOW      0xFFE0  // (255, 255, 0)
#define GRAY        0x9CD3  // (151, 151, 151) approximate
#define PURPLE      0xF81F  // (250, 0, 255) approximate
#define BLUE_LIGHT  0x051D  // (0, 200, 244) approximate
#define GREEN_DARK  0x0360  // (0, 135, 0) approximate
#define ORANGE      0xFC00  // (255, 128, 0) approximate - note: user's RGB(128,184,253) is actually light blue
