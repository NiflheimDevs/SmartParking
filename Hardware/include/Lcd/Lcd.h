#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"


void setupLCD();
void printCenter(String text);
void showRFIDAuthorized(int parkingSlot);
void showRFIDNotAuthorized();
void showExitPrice(String price);
void showParkingStatus();


