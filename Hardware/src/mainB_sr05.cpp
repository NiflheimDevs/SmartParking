#include <Arduino.h>
#include "mesh.h"
#include "sensors/ultrasonic_sensor.h"
#include "Led/Led.h"
#include "config.h"


void mesh_send_parking() {
    JSONVar obj;
    obj["type"] = "parking";

    JSONVar spots;
    for (int i = 0; i < PARKING_SPACES; i++)
        spots[i] = isParkingSpaceOccupied(i) ? 1 : 0;

    obj["spots"] = spots;

    mesh.sendBroadcast(JSON.stringify(obj));
}


void mesh_handle_received(JSONVar obj) {}

void forward_to_server(JSONVar obj) {
    // later: MQTT upload to backend
}

void setup() {
    Serial.begin(115200);
    setupAllUltrasonicSensors();
    // setupLEDStrip();
    mesh_init();
}

void loop() {
    mesh_update();

    static unsigned long last = 0;
    if (millis() - last > 1000) {
        mesh_send_parking();
        last = millis();
    }
}



// #include <Arduino.h>
// #include "mesh.h"
// #include "sensors/ultrasonic_sensor.h"
// #include "Led/Led.h"
// #include "config.h"

// // ===== Implement required mesh callbacks =====

// uint8_t read_parking(int index) {
//     return isParkingSpaceOccupied(index);
// }

// uint8_t read_rfid() {
//     return 0xFF;   // parking node has no RFID
// }

// void gate_set_open(uint8_t open) { 
//     // nothing to do on the parking unit
// }

// void update_leds(uint8_t occ[7]) {
//     updateParkingSpaceLEDs();
// }

// // ===== SETUP =====
// void setup() {
//     Serial.begin(115200);
//     Serial.println("PARKING NODE START");

//     MY_ROLE = NODE_ROLE_PARKING;

//     setupAllUltrasonicSensors();
//     setupLEDStrip();

//     mesh_init();
//     mesh_start_tasks();
// }

// void loop() {}
