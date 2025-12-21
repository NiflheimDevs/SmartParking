// #include <Arduino.h>
// #include "mesh.h"
// #include "sensors/ultrasonic_sensor.h"
// #include "Led/Led.h"
// #include "config.h"

// // Send parking data to mesh
// void mesh_send_parking() {
//     JSONVar obj;
//     obj["type"] = "parking";

//     JSONVar spots;
//     for (int i = 0; i < PARKING_SPACES; i++) {
//         spots[i] = isParkingSpaceOccupied(i) ? 1 : 0;
//     }
//     obj["spots"] = spots;

//     mesh.sendBroadcast(JSON.stringify(obj));
// }

// // Parking node does not react locally to mesh messages yet
// void mesh_handle_received(JSONVar obj) {
//     // INTENTIONALLY EMPTY
//     // Future: maybe handle "server_reply" or diagnostics
// }

// // Parking node forwards to server ONLY if root
// void forward_to_server(JSONVar obj) {
//     // PLACEHOLDER
//     // root → server upload
//     // root also → broadcast server_reply when server responds

//     // Example for later implementation:
//     // String payload = JSON.stringify(obj);
//     // http_post("/parking", payload);
//     // mqtt_publish("smartparking/data", payload);
// }

// void setup() {
//     Serial.begin(115200);
//     Serial.println("[Parking] Node start");

//     setupAllUltrasonicSensors();
//     setupLEDStrip();

//     mesh_init();
// }

// void loop() {
//     mesh_update();

//     static unsigned long last = 0;
//     if (millis() - last > 1000) {
//         mesh_send_parking();
//         last = millis();
//     }
// }


