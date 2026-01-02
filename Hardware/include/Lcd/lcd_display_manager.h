#ifndef LCD_DISPLAY_MANAGER_H
#define LCD_DISPLAY_MANAGER_H

#include "config.h"

// Initialize the LCD display manager
void initLCDDisplayManager();

// Notify that RFID activity occurred (entrance or exit)
void notifyRFIDActivity();

// Update the display manager (call this in main loop)
void updateLCDDisplayManager();

#endif

