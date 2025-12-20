#include "mqtt_client.h"
#include "config.h"



void connect() {
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.print("\nconnecting...");
    while (!client.connect("esp32ClientID") )
    {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.println("\nconnected!");
  
    client.subscribe("parking/entrance/response");   
    client.subscribe("parking/exit/response");
    client.subscribe("parking/control");
  
}

bool initWiFi()
{
    WiFi.begin(ssid, pass);

    // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
    // by Arduino. You need to set the IP address directly.
    client.begin("api.smartparking.niflheimdevs.ir",1883, net);
 
    connect();
    return WiFi.status() == WL_CONNECTED && client.connected();
}
