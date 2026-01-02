#ifndef ESP32_S3_H
#define ESP32_S3_H

#include "config.h"
#include "motors/servo_control.h"
#include "RFID/RFID_reader.h"
#include "Led/Led.h"
#include "MQTT/mqtt.h"
#include "mesh/espnow_mesh.h"

void setupS3();
void Monitor();

#endif
