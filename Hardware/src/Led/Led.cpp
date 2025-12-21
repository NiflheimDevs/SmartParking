#include "Led/Led.h"
#include "config.h"
#include "sensors/ultrasonic_sensor.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Static variable to track RFID status LED state
static unsigned long rfidStatusEndTime = 0;
static bool rfidStatusActive = false;

void setupLEDStrip() {
    strip.begin();
    strip.setBrightness(LED_BRIGHTNESS);
    strip.clear();
    strip.show();
    Serial.println("LED Strip initialized with " + String(LED_COUNT) + " LEDs");
}

void updateParkingSpaceLEDs() {
    // Check if RFID status is currently showing (blue), if so, don't update
    if (rfidStatusActive && millis() < rfidStatusEndTime) {
        return;  // Keep showing blue LED
    }
    
    // RFID status has expired, update parking space LEDs
    if (rfidStatusActive && millis() >= rfidStatusEndTime) {
        rfidStatusActive = false;
    }
    
    // Update each LED based on parking space occupancy
    for (int i = 0; i < PARKING_SPACES && i < LED_COUNT; i++) {
        bool isOccupied = isParkingSpaceOccupied(i);
        uint32_t color = isOccupied ? COLOR_RED : COLOR_GREEN;
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void showRFIDStatus(bool isAuthorized) {
    if (isAuthorized) {
        // Show blue on all LEDs for entry response
        for (int i = 0; i < LED_COUNT; i++) {
            strip.setPixelColor(i, COLOR_BLUE);
        }
        strip.show();
        
        // Set timer to show blue for 1 second
        rfidStatusActive = true;
        rfidStatusEndTime = millis() + 1000;
    }
    // If not authorized, do nothing - let parking space LEDs show normally
}

