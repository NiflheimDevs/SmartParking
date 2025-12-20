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

