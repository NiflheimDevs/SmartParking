#ifndef ESP32_H
#define ESP32_H

#include "config.h"
#include "sensors/ultrasonic_sensor.h"
#include "MQTT/mqtt.h"
#include <Arduino_JSON.h>

void initParkingMonitor();
void updateParkingMonitor();

#endif
