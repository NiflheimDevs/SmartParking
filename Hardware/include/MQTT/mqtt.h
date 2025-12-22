#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>
#include <Arduino_JSON.h>
#include "config.h"
#include "motors/servo_control.h"

extern WiFiClient net;
extern MQTTClient client;

// Initialize WiFi connection
bool initWiFi();
void connect();

// MQTT message callback handler
void messageReceived(String &topic, String &payload);
void PublishRFID(String cardUID,String topic);