// #include "mesh.h"
// #include "config.h"

// static const char *TAG = "MESH";

// #define ROUTER_SSID "YourWiFi"
// #define ROUTER_PASS "YourPassword"

// #define ROOT_REELECT_INTERVAL (5 * 60 * 1000)

// node_role_t MY_ROLE = NODE_ROLE_PARKING;

// static void mesh_event_handler(mesh_evt_t event){
//     switch (event.id)
//     {
//     case MESH_EVENT_STARTED:
//         ESP_LOGI(TAG,"MESH started");
//         break;
//     case MESH_EVENT_PARENT_CONNECTED:
//         ESP_LOGI(TAG,"Parent connected");
//         break;
//     case MESH_EVENT_PARENT_DISCONNECTED:
//         ESP_LOGW(TAG,"Parent disconnected");
//         break;
//     default:
//         break;
//     }
// }


// static void mesh_root_task(void *param){
//     while(true){
//         // The only reason I used a FreeRTOS task (xTaskCreate) is because ESP-MESH examples from Espressif use tasks, and tasks keep things non-blocking.
//         vTaskDelay(ROOT_REELECT_INTERVAL/portTICK_PERIOD_MS);
//         if(esp_mesh_is_root()){
//             ESP_LOGI(TAG, "I am the root node");
//                                 //   MESH_VOTE_REASON_ROOT_CONFLICT   
//             esp_mesh_waive_root(NULL,MESH_VOTE_REASON_ROOT_INITIATED);
//         }
//     }
// }


// static void mesh_rx_task(void *param){
//     mesh_data_t data;
//     mesh_addr_t from;
//     int flag;

//     uint8_t buf[64];
//     data.data = buf;

//     while(true){
//         data.size = sizeof(buf);

//         if(esp_mesh_recv(&from, &data, portMAX_DELAY, &flag, NULL) != ESP_OK)
//             continue;
        
//         char type = buf[0];

//         if(type == 'P'){
//             uint8_t occ[7];
//             for(int i =0; i<7;i++)
//                 occ[i] = (buf[i+1] == '1');
//             if(MY_ROLE == NODE_ROLE_PARKING)
//                 update_leds(occ);
//         }
//         else if(type == 'R'){
//             uint8_t ok = (buf[1] == '1');
//             if(MY_ROLE == NODE_ROLE_GATE)
//                 gate_set_open(ok);
//         }
//     }
// }

// static void mesh_tx_task(void *param){
//     mesh_data_t data;
//     data.proto = MESH_PROTO_BIN;
//     data.tos = MESH_TOS_P2P;

//     uint8_t buf[64];
//     data.data = buf;

//     mesh_addr_t root_addr;

//     while(true){
//         esp_mesh_get_root(&root_addr);

//         if(MY_ROLE == NODE_ROLE_PARKING){
//             buf[0] = 'P';
//             for(int i = 0 ; i<PARKING_SPACES; i++)
//                 buf[i+1] = read_parking(i) ? '1' : '0';
            
//             data.size = 1+PARKING_SPACES;
//             esp_mesh_send(&root_addr, &data,portMAX_DELAY,NULL,0);
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
//         } 
//         else if(MY_ROLE == NODE_ROLE_GATE){
//             uint8_t auth = read_rfid();
//             // if(auth != 0xFF){
//             // }
//             buf[0] = 'R';
//             buf[1] = auth ? '1' : '0';
//             data.size =2;
//             esp_mesh_send(&root_addr, &data,portMAX_DELAY,NULL,0);
//             vTaskDelay(200 / portTICK_PERIOD_MS);
//         }
//     }
// }

// void mesh_init()
// {
//     WiFi.mode(WIFI_MODE_STA);

//     mesh_cfg_t cfg = MESH_INIT_CONFIG_DEFAULT();
//     memcpy(&cfg.router.ssid, ROUTER_SSID, strlen(ROUTER_SSID));
//     memcpy(&cfg.router.password, ROUTER_PASS, strlen(ROUTER_PASS));

//     uint8_t mesh_id[6] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
//     memcpy(cfg.mesh_id, mesh_id, 6);

//     cfg.channel = 6;
//     cfg.crypto_funcs = &g_wifi_default_mesh_crypto_funcs;

//     esp_mesh_init();
//     esp_mesh_set_config(&cfg);
//     esp_mesh_set_self_organized(true, false);
//     esp_mesh_start();

//     ESP_LOGI(TAG, "Mesh initialized");
// }

// void mesh_start_tasks()
// {
//     xTaskCreate(mesh_rx_task  , "mesh_rx"  , 4096, NULL, 5, NULL);
//     xTaskCreate(mesh_tx_task  , "mesh_tx"  , 4096, NULL, 5, NULL);
//     xTaskCreate(mesh_root_task, "mesh_root", 4096, NULL, 5, NULL);
// }


#include "mesh.h"

painlessMesh mesh;

bool isRoot = false;
int myRSSI = -100;
int otherRSSI = -100;

unsigned long lastRSSISend = 0;
const unsigned long RSSI_INTERVAL = 5 * 60 * 1000; 


void receivedCallback(uint32_t from, String &msg) {
    JSONVar obj = JSON.parse(msg);
    if (JSON.typeof(obj) == "undefined") return;

    String type = (const char*) obj["type"];

    // RSSI EXCHANGE
    if (type == "rssi") {
        otherRSSI = (int)obj["value"];

        // Recalculate root
        isRoot = (myRSSI > otherRSSI);

        Serial.printf("[MESH] MyRSSI=%d  OtherRSSI=%d  → isRoot=%d\n",
            myRSSI, otherRSSI, isRoot);
    }
    // FORWARD TO SERVER IF ROOT
    if (isRoot) {
        forward_to_server(obj);
    }
    // Send app-level data to node handler
    mesh_handle_received(obj);
}

void mesh_send_rssi() {
    JSONVar obj;
    obj["type"]  = "rssi";
    obj["value"] = myRSSI;

    mesh.sendBroadcast(JSON.stringify(obj));
}

void mesh_init() {
    mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    Serial.println("[Mesh] Initialized");
}

void mesh_update() {
    mesh.update();

    static unsigned long lastRSSIMeas = 0;

    // Measure RSSI every second
    if (millis() - lastRSSIMeas > 1000) {
        myRSSI = WiFi.RSSI();
        lastRSSIMeas = millis();
    }

    // Send RSSI every 5 min
    if (millis() - lastRSSISend > RSSI_INTERVAL) {
        mesh_send_rssi();
        lastRSSISend = millis();
    }
}

// Default weak functions
__attribute__((weak)) void mesh_send_parking() {}
__attribute__((weak)) void mesh_send_rfid(String uid) {}
__attribute__((weak)) void forward_to_server(JSONVar obj) {}
__attribute__((weak)) void mesh_handle_received(JSONVar obj) {}