#include "mesh.h"

painlessMesh mesh;

bool isRoot = false;
int myRSSI = -100;
int otherRSSI = -100;

unsigned long lastRSSISend = 0;
const unsigned long RSSI_INTERVAL = 5 * 60 * 1000; // 5 minutes


// Mesh message handler
void receivedCallback(uint32_t from, String &msg) {
    JSONVar obj = JSON.parse(msg);
    if (JSON.typeof(obj) == "undefined") {
        Serial.println("[Mesh] JSON parse failed");
        return;
    }

    String type = (const char*) obj["type"];

    // ---------- RSSI messages ----------
    if (type == "rssi") {
        otherRSSI = (int)obj["value"];
        isRoot = (myRSSI > otherRSSI);
    }

    // ---------- ROOT forwards to server ----------
    if (isRoot) {
        forward_to_server(obj);
    }

    // ---------- Node-specific handler ----------
    mesh_handle_received(obj);
}


// Broadcast my RSSI
void mesh_send_rssi() {
    JSONVar obj;
    obj["type"]  = "rssi";
    obj["value"] = myRSSI;

    mesh.sendBroadcast(JSON.stringify(obj));
}


// Init mesh
void mesh_init() {
    mesh.setDebugMsgTypes(ERROR | STARTUP);
    mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallback);

    Serial.println("[Mesh] Initialized");
}


// Mesh update loop
void mesh_update() {
    mesh.update();

    myRSSI = WiFi.RSSI();

    // Send RSSI every 5 minutes
    if (millis() - lastRSSISend > RSSI_INTERVAL) {
        mesh_send_rssi();
        lastRSSISend = millis();
    }
}


__attribute__((weak)) void mesh_send_parking() {}
__attribute__((weak)) void mesh_send_rfid(String uid) {}
__attribute__((weak)) void forward_to_server(JSONVar obj) {}
__attribute__((weak)) void mesh_handle_received(JSONVar obj) {}