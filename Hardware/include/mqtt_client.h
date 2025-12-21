

#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

extern WiFiClient net;
extern MQTTClient client;

// Initialize WiFi connection
bool initWiFi();
void connect();

// MQTT message callback handler
void messageReceived(String &topic, String &payload);

// Gate timer management (call from main loop)
void updateGateTimers();
