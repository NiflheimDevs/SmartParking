

#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient client;

// Initialize WiFi connection
bool initWiFi();
void connect();
