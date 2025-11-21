#include <Arduino.h>
#include "mesh.h"
#include "RFID/RFID_reader.h"
#include "motors/servo_control.h"
#include "Led/Led.h"
#include "config.h"

void mesh_send_rfid(String uid) {
    JSONVar obj;
    obj["type"] = "rfid";
    obj["uid"] = uid;

    mesh.sendBroadcast(JSON.stringify(obj));
}

// Gate node does NOT send parking
void mesh_send_parking() {}

void mesh_handle_received(JSONVar obj) {
    String type = (const char*) obj["type"];

    if (type == "server_reply") {
        bool openGate = (bool)obj["open"];
        Serial.printf("Server says OPEN=%d\n", openGate);
        setServoAngle(openGate ? SERVO_OPEN_ANGLE : SERVO_CLOSED_ANGLE);
    }
}

void forward_to_server(JSONVar obj) {
    // EMPTY — implement later
}


void setup() {
    Serial.begin(115200);
    setupRFID(RFID_SS_PIN, RFID_RST_PIN);
    setupServo(SERVO_PIN);
    mesh_init();
}

void loop() {
    mesh_update();

    static unsigned long lastCheck = 0;
    const unsigned long RFID_CHECK_INTERVAL = 200;
    String uid;
    if (millis() - lastCheck >= RFID_CHECK_INTERVAL) {
        lastCheck = millis();
        String uid;
        if (isAuthorizedCard(uid)) {
            Serial.println("RFID detected, sending to root...");
            mesh_send_rfid(uid);
        }
    }
}







// String uid;

// // ===== Implement mesh callbacks =====

// uint8_t read_parking(int index) {
//     return 0;  // gate unit does not have ultrasonic sensors
// }

// uint8_t read_rfid() {
//     if (isAuthorizedCard(uid))
//         return 1;
//     return 0xFF; // no card scanned yet
// }

// void gate_set_open(uint8_t ok) {
//     Serial.println(ok ? "OPENING GATE" : "GATE CLOSED");
//     setServoAngle(ok ? SERVO_OPEN_ANGLE : SERVO_CLOSED_ANGLE);
//     showRFIDStatus(ok);
// }

// void update_leds(uint8_t occ[7]) {
//     updateParkingSpaceLEDs();
// }

// // ===== SETUP =====
// void setup() {
//     Serial.begin(115200);
//     Serial.println("GATE NODE START");

//     MY_ROLE = NODE_ROLE_GATE;

//     setupRFID(RFID_SS_PIN, RFID_RST_PIN);
//     setupServo(SERVO_PIN);
//     setupLEDStrip();

//     mesh_init();
//     mesh_start_tasks();
// }

// void loop() {}
