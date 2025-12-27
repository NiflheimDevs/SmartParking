#ifndef ESPNOW_MESH_H
#define ESPNOW_MESH_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "config.h"
#include "MQTT/mqtt.h"
#include "MQTT/mqtt_handler.h"
#include <Arduino_JSON.h>


// ESP-NOW Mesh Configuration
#define ESPNOW_CHANNEL 1
#define RSSI_CHECK_INTERVAL 300000  // 5 minutes in milliseconds
#define RSSI_THRESHOLD -80          // RSSI threshold for connection quality

// Device roles
enum DeviceRole {
    ROLE_AP,      // Access Point - connects to WiFi/MQTT
    ROLE_STATION   // Station - sends data via ESP-NOW to AP
};

// ESP-NOW data structure
typedef struct struct_message {
    uint8_t msgType;        // 0 = RSSI check, 1 = data packet
    int8_t rssi;            // RSSI value
    uint8_t deviceId;        // Device identifier (0 = ESP32-S3, 1 = NodeMCU)
    char data[200];         // Data payload
} struct_message;

// Function declarations
void initESPNOWMesh();
void checkRSSIAndSwitchRole();
bool isAPRole();
DeviceRole getCurrentRole();
void sendDataViaMesh(const char* data);
void setPeerMacAddress(uint8_t* mac);
void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);
void onDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len);
int8_t getRSSI();
void switchToAPRole();
void switchToStationRole();
void updateMesh();  // Call this in loop() to handle periodic RSSI checks
void printDeviceMAC();  // Print device MAC address for configuration
uint8_t getDeviceId();  // Get device ID (0 = ESP32-S3, 1 = NodeMCU)

#endif

