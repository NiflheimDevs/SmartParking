// #include <Arduino.h>
// #include "mesh.h"
// #include "RFID/RFID_reader.h"
// #include "motors/servo_control.h"
// #include "Led/Led.h"
// #include "config.h"


// // Gate node sends RFID UID to mesh
// void mesh_send_rfid(String uid) {
//     JSONVar obj;
//     obj["type"] = "rfid";
//     obj["uid"]  = uid;

//     mesh.sendBroadcast(JSON.stringify(obj));
// }

// void mesh_send_parking() {} // gate does not send parking


// // Gate node does not react locally yet
// // Will react later when server_reply is implemented
// void mesh_handle_received(JSONVar obj) {
//     // INTENTIONALLY EMPTY
//     // Future: open gate based on "server_reply"
// }


// // Gate node forwards to server ONLY if root
// void forward_to_server(JSONVar obj) {
//     // PLACEHOLDER
//     // root → send to server
//     // when server replies → broadcast "server_reply"
// }


// void setup() {
//     Serial.begin(115200);

//     setupRFID(RFID_SS_PIN, RFID_RST_PIN);
//     setupServo(SERVO_PIN);
    
//     mesh_init();
// }

// void loop() {
//     mesh_update();

//     static unsigned long lastCheck = 0;
//     if (millis() - lastCheck >= 200) {
//         lastCheck = millis();

//         String uid;
//         if (isAuthorizedCard(uid)) {
//             mesh_send_rfid(uid);
//         }
//     }
// }