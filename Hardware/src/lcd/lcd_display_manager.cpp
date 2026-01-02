#include "Lcd/lcd_display_manager.h"
#include "Lcd/Lcd.h"

// Configuration constants
static const unsigned long IDLE_DISPLAY_DELAY = 5000;  // Show parking status after 5 seconds of inactivity
static const unsigned long IDLE_UPDATE_INTERVAL = 2000; // Update parking status every 2 seconds when idle

// State variables
static unsigned long lastRFIDActivity = 0;
static unsigned long lastIdleUpdate = 0;
static bool showingIdleStatus = false;
static bool isInitialized = false;

void initLCDDisplayManager() {
    if (isInitialized) return;
    
    lastRFIDActivity = millis();
    lastIdleUpdate = 0;
    showingIdleStatus = false;
    isInitialized = true;
    
    // Show initial parking status after a short delay
    delay(1000);
    showParkingStatus();
    showingIdleStatus = true;
    lastIdleUpdate = millis();
}

void notifyRFIDActivity() {
    lastRFIDActivity = millis();
    showingIdleStatus = false;
}

void updateLCDDisplayManager() {
    if (!isInitialized) return;
    
    unsigned long currentTime = millis();
    unsigned long timeSinceLastActivity = currentTime - lastRFIDActivity;
    
    // Check if we should show idle status
    if (timeSinceLastActivity >= IDLE_DISPLAY_DELAY) {
        if (!showingIdleStatus) {
            // Transition to idle state
            showParkingStatus();
            showingIdleStatus = true;
            lastIdleUpdate = currentTime;
        } else if (currentTime - lastIdleUpdate >= IDLE_UPDATE_INTERVAL) {
            // Update parking status periodically when idle
            showParkingStatus();
            lastIdleUpdate = currentTime;
        }
    }
}

