#include "Led/Led.h"
#include "config.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setupLEDStrip() {
    strip.begin();
    strip.setBrightness(LED_BRIGHTNESS);
    strip.clear();
    strip.show();
    Serial.println("LED Strip initialized with " + String(LED_COUNT) + " LEDs");
}

void setAllLEDs(uint32_t color) {
    for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void setLED(uint8_t ledIndex, uint32_t color) {
    if (ledIndex < LED_COUNT) {
        strip.setPixelColor(ledIndex, color);
        strip.show();
    }
}

void setLEDBrightness(uint8_t brightness) {
    strip.setBrightness(brightness);
    strip.show();
}

void clearAllLEDs() {
    strip.clear();
    strip.show();
}

void showLEDs() {
    strip.show();
}

void blinkLEDs(uint32_t color, uint16_t delayMs, uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        setAllLEDs(color);
        delay(delayMs);
        clearAllLEDs();
        delay(delayMs);
    }
}

void rainbowCycle(uint16_t delayMs) {
    for (uint16_t j = 0; j < 256 * 5; j++) {
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV((i * 65536L / LED_COUNT) + j)));
        }
        strip.show();
        delay(delayMs);
    }
}

void colorWipe(uint32_t color, uint16_t delayMs) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, color);
        strip.show();
        delay(delayMs);
    }
}

void theaterChase(uint32_t color, uint16_t delayMs) {
    for (int j = 0; j < 10; j++) {
        for (int q = 0; q < 3; q++) {
            for (uint8_t i = 0; i < LED_COUNT; i = i + 3) {
                strip.setPixelColor(i + q, color);
            }
            strip.show();
            delay(delayMs);
            for (uint8_t i = 0; i < LED_COUNT; i = i + 3) {
                strip.setPixelColor(i + q, 0);
            }
        }
    }
}

void showParkingStatus(bool isOccupied) {
    if (isOccupied) {
        setAllLEDs(COLOR_RED);
        Serial.println("🚗 Parking space occupied - Red LED");
    } else {
        setAllLEDs(COLOR_GREEN);
        Serial.println("✅ Parking space available - Green LED");
    }
}

void showRFIDStatus(bool isAuthorized) {
    if (isAuthorized) {
        blinkLEDs(COLOR_GREEN, 200, 3);
        Serial.println("✅ RFID authorized - Green blink");
    } else {
        blinkLEDs(COLOR_RED, 200, 3);
        Serial.println("❌ RFID unauthorized - Red blink");
    }
}

void showSystemStatus(bool isReady) {
    if (isReady) {
        setAllLEDs(COLOR_BLUE);
        delay(500);
        clearAllLEDs();
        Serial.println("🔧 System ready - Blue flash");
    } else {
        setAllLEDs(COLOR_YELLOW);
        delay(500);
        clearAllLEDs();
        Serial.println("⚠️ System not ready - Yellow flash");
    }
}

void showAllParkingSpacesStatus() {
    for (int i = 0; i < PARKING_SPACES; i++) {
        extern bool isParkingSpaceOccupied(int spaceIndex);
        bool occupied = isParkingSpaceOccupied(i);
        showParkingSpaceStatus(i, occupied);
    }
}

void showParkingSpaceStatus(int spaceIndex, bool isOccupied) {
    if (spaceIndex >= 0 && spaceIndex < PARKING_SPACES) {
        if (isOccupied) {
            setLED(spaceIndex, COLOR_RED);
        } else {
            setLED(spaceIndex, COLOR_GREEN);
        }
    }
}

void updateParkingSpaceLEDs() {
    for (int i = 0; i < PARKING_SPACES; i++) {
        extern bool isParkingSpaceOccupied(int spaceIndex);
        bool occupied = isParkingSpaceOccupied(i);
        showParkingSpaceStatus(i, occupied);
    }
    
    extern int getAvailableParkingSpaces();
    int availableCount = getAvailableParkingSpaces();
    showAvailableSpacesCount(availableCount);
}

void showAvailableSpacesCount(int availableCount) {
    Serial.println("Available parking spaces: " + String(availableCount) + "/" + String(PARKING_SPACES));
    
    if (availableCount == 0) {
        showParkingLotFull();
    } else if (availableCount <= 2) {
        blinkLEDs(COLOR_YELLOW, 200, 2);
    }
}

void showParkingLotFull() {
    blinkLEDs(COLOR_RED, 300, 3);
    Serial.println("🚨 Parking lot is FULL!");
}
