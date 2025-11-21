#ifndef MESH_H
#define MESH_H

#include <Arduino.h>
#include <painlessMesh.h>
#include <Arduino_JSON.h>

#define MESH_SSID      "SmartParkingMesh"
#define MESH_PASSWORD  "12345678"
#define MESH_PORT      5555

extern painlessMesh mesh;

extern bool isRoot;          
extern int myRSSI;
extern int otherRSSI;

// Node-specific send functions
void mesh_send_parking();
void mesh_send_rfid(String uid);

// Root → server forwarder
void forward_to_server(JSONVar obj);

// Receiver (parking/gate specific)
void mesh_handle_received(JSONVar obj);

// Core
void mesh_init();
void mesh_update();

#endif




// #ifndef MESH_H
// #define MESH_H

// #include <Arduino.h>
// #include "esp_mesh.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "esp_system.h"

// typedef enum {
//     NODE_ROLE_PARKING = 0,
//     NODE_ROLE_GATE    = 1
// } node_role_t;

// extern node_role_t MY_ROLE;

// uint8_t read_parking(int index);
// uint8_t read_rfid();
// void gate_set_open(uint8_t open);
// void update_leds(uint8_t occ[7]);

// void mesh_init();
// void mesh_start_tasks();

// #endif
