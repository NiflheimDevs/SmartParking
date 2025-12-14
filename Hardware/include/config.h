#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG true

#define SERVO_EXIT_PIN 5
#define SERVO_ENTRY_PIN 6

#define PARKING_SPACES 7

// Ultrasonic sensor pins for each parking space
// Space 0: Pins 12, 14
// Space 1: Pins 13, 15  
// Space 2: Pins 16, 17
// Space 3: Pins 18, 19
// Space 4: Pins 22, 23
// Space 5: Pins 25, 26
// Space 6: Pins 27, 32

// Ultrasonic sensor pin configurations for 7 parking spaces
#define ULTRASONIC_TRIG_PIN_0 13
#define ULTRASONIC_ECHO_PIN_0 12

#define ULTRASONIC_TRIG_PIN_1 14
#define ULTRASONIC_ECHO_PIN_1 27

#define ULTRASONIC_TRIG_PIN_2 26
#define ULTRASONIC_ECHO_PIN_2 25

#define ULTRASONIC_TRIG_PIN_3 33
#define ULTRASONIC_ECHO_PIN_3 32

#define ULTRASONIC_TRIG_PIN_4 5
#define ULTRASONIC_ECHO_PIN_4 17

#define ULTRASONIC_TRIG_PIN_5 23
#define ULTRASONIC_ECHO_PIN_5 22

#define ULTRASONIC_TRIG_PIN_6 19
#define ULTRASONIC_ECHO_PIN_6 18

// SPI Pins for RFID readers
#define SPI_SCK_PIN  36
#define SPI_MISO_PIN 37
#define SPI_MOSI_PIN 35

// Entry RFID reader pins
#define RFID_ENTRY_SS_PIN  11
#define RFID_ENTRY_RST_PIN 47

// Exit RFID reader pins
#define RFID_EXIT_SS_PIN  8
#define RFID_EXIT_RST_PIN 47

// Legacy support (keeping for compatibility)
#define RFID_SS_PIN RFID_ENTRY_SS_PIN
#define RFID_RST_PIN RFID_ENTRY_RST_PIN

#define LED_PIN 48
#define LED_COUNT 7
#define LED_BRIGHTNESS 50

#define PARKING_THRESHOLD 17
#define SERVO_OPEN_ANGLE 180
#define SERVO_CLOSED_ANGLE 0
#define SENSOR_READ_INTERVAL 500

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// MQTT Configuration
#define MQTT_BROKER ""  // Change to your MQTT broker
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "SmartParking_ESP32"
#define MQTT_TOPIC_RFID "parking/entrance"
#define MQTT_TOPIC_RESPONSE "parking/entrance/response"
#define MQTT_API_ENDPOINT "api.smartparking.niflheimsdevs.ir"

#endif
