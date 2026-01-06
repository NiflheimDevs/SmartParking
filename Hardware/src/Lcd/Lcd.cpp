#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <MQTT.h>
#include <Arduino_JSON.h>

// =====================
// WiFi
// =====================
const char* ssid = "Saman's S24 Ultra";
const char* pass = "2334Saman92";

// =====================
// MQTT
// =====================
#define MQTT_BROKER "api.smartparking.niflheimdevs.ir"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP32_SLOTIFY"

#define ENTRANCE_RESPONSE_TOPIC "parking/entrance/response"
#define EXIT_RESPONSE_TOPIC     "parking/exit/response"
#define GATE_CONTROL_TOPIC      "parking/gate/control"
#define SPACE_TOPIC             "parking/space"

// =====================
// WiFi & MQTT
// =====================
WiFiClient net;
MQTTClient client;

// =====================
// TFT Pins
// =====================
#define TFT_CS   22
#define TFT_DC   1
#define TFT_RST  23

#define SPI_TFT_SCK_PIN   21
#define SPI_TFT_MISO_PIN  -1
#define SPI_TFT_MOSI_PIN  3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// =====================
// Parking Spaces
// =====================
#define NUM_SPACES 7
bool parkingOccupied[NUM_SPACES];   
unsigned long lastSpaceUpdate = 0;  

// =====================
// TFT Helpers
// =====================
void clearScreen() {
    tft.fillScreen(ST77XX_BLACK);
}

// Center text helper
void showCentered(String text, uint16_t color, int size, int yPos) {
    tft.setTextColor(color);
    tft.setTextSize(size);

    int16_t x, y;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
    tft.setCursor((tft.width() - w) / 2, yPos);
    tft.print(text);
}

// Splash screen
void showSlotify() {
    clearScreen();
    showCentered("SLOTIFY", ST77XX_CYAN, 3, tft.height()/2 - 20);
}

// =====================
// Draw Parking Space Box
// =====================
void drawSpaceBox(int index, bool occupied, int x, int y) {
    int boxWidth = 25;
    int boxHeight = 25;
    uint16_t color = occupied ? ST77XX_RED : ST77XX_GREEN;
    tft.fillRect(x, y, boxWidth, boxHeight, color);
    tft.drawRect(x, y, boxWidth, boxHeight, ST77XX_WHITE);

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.setCursor(x + 5, y + boxHeight + 2);
    tft.print("P" + String(index + 1));
}

// =====================
// Message Handlers
// =====================
void handleEntrance(JSONVar json) {
    clearScreen();
    bool authorized = json.hasOwnProperty("exist") && bool(json["exist"]) && String(json["error"]) == "";

    if (!authorized) {
        showCentered("UNAUTHORIZED", ST77XX_RED, 2, tft.height()/2 - 10);
        return;
    }

    showCentered("WELCOME!", ST77XX_GREEN, 2, 10);

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 40);
    tft.print("Owner: " + String(json["owner"]));

    tft.setCursor(10, 60);
    tft.print("Spot: " + String(int(json["parking_spot"])));
    lastSpaceUpdate = millis(); 

}

void handleExit(JSONVar json) {
    clearScreen();
    bool authorized = String(json["error"]) == "";

    if (!authorized) {
        showCentered("UNAUTHORIZED", ST77XX_RED, 2, tft.height()/2 - 10);
        return;
    }

    // Centered Goodbye
    showCentered("GOOD BYE!", ST77XX_YELLOW, 2, 20);

    // Centered Price
    String priceText = "Price: " + String(int(json["price"])) + " T";
    showCentered(priceText, ST77XX_WHITE, 1, 60);
    lastSpaceUpdate = millis(); 

}

void handleGate(JSONVar json) {
    clearScreen();

    String gateName = String(json["gate"]);
    bool stateBool = bool(json["state"]);
    String stateStr = stateBool ? "OPEN" : "CLOSED";
    uint16_t stateColor = stateBool ? ST77XX_GREEN : ST77XX_RED;

    showCentered("Gate:" + gateName, ST77XX_WHITE, 2, 10);
    showCentered("State: " + stateStr, stateColor, 2, 50);
    lastSpaceUpdate = millis(); 

}

// =====================
// Parking Space Updates
// =====================
void handleSpace(JSONVar json) {
    String spaceNumber = String(json["space_number"]); 
    bool occupied = bool(json["is_occupied"]);

    if (spaceNumber.length() < 2) return;
    int index = spaceNumber.substring(1).toInt() - 1; 
    if (index >= 0 && index < NUM_SPACES) {
        parkingOccupied[index] = occupied;
    }

    lastSpaceUpdate = millis(); 
}

// =====================
// Show All Parking Spaces Two Rows
// =====================
void showParkingSpaces() {
    clearScreen();
    tft.setTextSize(1);

    int marginX = 10;
    int marginY = 20;
    int boxWidth = 25;
    int boxHeight = 25;
    int spacingX = 15;
    int spacingY = 40;

    for (int i = 0; i < NUM_SPACES; i++) {
        int row = i < 4 ? 0 : 1;
        int col = i % 4;
        int x = marginX + col * (boxWidth + spacingX);
        int y = marginY + row * spacingY;
        drawSpaceBox(i, parkingOccupied[i], x, y);
    }
}

// =====================
// MQTT Callback
// =====================
void messageReceived(String &topic, String &payload) {
    JSONVar json = JSON.parse(payload);
    if (JSON.typeof(json) == "undefined") return;

    if (topic == ENTRANCE_RESPONSE_TOPIC) handleEntrance(json);
    else if (topic == EXIT_RESPONSE_TOPIC) handleExit(json);
    else if (topic == GATE_CONTROL_TOPIC) handleGate(json);
    else if (topic == SPACE_TOPIC) handleSpace(json);
}

// =====================
// MQTT Connect
// =====================
void connectMQTT() {
    Serial.print("Connecting to MQTT");
    while (!client.connect(MQTT_CLIENT_ID)) delay(1000);
    Serial.println("\nMQTT connected");

    client.subscribe(ENTRANCE_RESPONSE_TOPIC);
    client.subscribe(EXIT_RESPONSE_TOPIC);
    client.subscribe(GATE_CONTROL_TOPIC);
    client.subscribe(SPACE_TOPIC);
}

// =====================
// Setup
// =====================
void setup() {
    Serial.begin(115200);

    // TFT Init
    SPI.begin(SPI_TFT_SCK_PIN, SPI_TFT_MISO_PIN, SPI_TFT_MOSI_PIN);
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    showSlotify();

    // Initialize parking array empty
    for (int i = 0; i < NUM_SPACES; i++) parkingOccupied[i] = false;

    // WiFi
    WiFi.begin(ssid, pass);
    Serial.print("Connecting WiFi");
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println("\nWiFi connected");

    // MQTT
    client.begin(MQTT_BROKER, MQTT_PORT, net);
    client.onMessage(messageReceived);
    connectMQTT();
}

// =====================
// Loop
// =====================
void loop() {
    client.loop();

    // Show parking space status if no update in 5 seconds
    if (millis() - lastSpaceUpdate > 5000) {
        showParkingSpaces();
        lastSpaceUpdate = millis(); 
    }
}
