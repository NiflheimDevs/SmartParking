#include "config.h"

#if DEVICE_TYPE == DEVICE_ESP32_S3
  #include "esp32_s3.h"
#elif DEVICE_TYPE == DEVICE_NODEMCU
  #include "esp32.h"
#endif

void setup() {
#if DEVICE_TYPE == DEVICE_ESP32_S3
    setupS3();                 // RFID + gates
#elif DEVICE_TYPE == DEVICE_NODEMCU
    initParkingMonitor();      // Sensors only
#endif
}

void loop() {
#if DEVICE_TYPE == DEVICE_ESP32_S3
    Monitor();                 // RFID + MQTT
#elif DEVICE_TYPE == DEVICE_NODEMCU
    updateParkingMonitor();    // Sensors + MQTT
#endif
}
