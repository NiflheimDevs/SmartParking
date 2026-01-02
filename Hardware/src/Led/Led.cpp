#include "Led/Led.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Static variable to track RFID status LED state
static unsigned long rfidStatusEndTime = 0;
static bool rfidStatusActive = false;

bool parkingSpaceOccupied[PARKING_SPACES] = {false};

void setupLEDStrip() {
    strip.begin();
    strip.setBrightness(LED_BRIGHTNESS);
    strip.clear();
    for (int i = 0; i < PARKING_SPACES && i < LED_COUNT; i++) {
        strip.setPixelColor(i, COLOR_GREEN);
    }
    strip.show();
    Serial.println("LED Strip initialized with " + String(LED_COUNT) + " LEDs");
}

void updateParkingSpaceLEDs() {
    // Update each LED based on parking space occupancy
    for (int i = 0; i < PARKING_SPACES && i < LED_COUNT; i++) {
        bool isOccupied = parkingSpaceOccupied[i];
        uint32_t color = isOccupied ? COLOR_RED : COLOR_GREEN;
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void updateParkingSpaceStatus(int spaceIndex, bool occupied) {
    if (spaceIndex < 0 || spaceIndex >= PARKING_SPACES) return;
    parkingSpaceOccupied[spaceIndex] = occupied;
}

void setParkingSpaceLED(int spaceIndex, uint32_t color) {
    if (spaceIndex < 0 || spaceIndex >= PARKING_SPACES || spaceIndex >= LED_COUNT) return;
    strip.setPixelColor(6-spaceIndex, color);
    strip.show();
}