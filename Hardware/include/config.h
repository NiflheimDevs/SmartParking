#include <Arduino.h>


#if ENABLE_RFID_SYSTEM
    #define BOARD_NAME "ESP32-S3 Feather"
#elif ENABLE_PARKING_MONITOR
    #define BOARD_NAME "NodeMCU ESP32"
#else
    #error "No device role defined!"
#endif

#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG true

#define SERVO_EXIT_PIN 5
#define SERVO_ENTRY_PIN 6

#define PARKING_SPACES 7

// Ultrasonic sensor pins for each parking space
// Space 0: Pins 13, 12
// Space 1: Pins 14, 27  
// Space 2: Pins 26, 25
// Space 3: Pins 33, 32
// Space 4: Pins 5, 17
// Space 5: Pins 23, 22
// Space 6: Pins 19, 18

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

#define LED_PIN 48
#define LED_COUNT 7
#define LED_BRIGHTNESS 50
#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_OFF     0x000000

#define PARKING_THRESHOLD 17
#define SERVO_OPEN_ANGLE 180
#define SERVO_CLOSED_ANGLE 0
#define SENSOR_READ_INTERVAL 500
#define GATE_OPEN_DURATION_MS 3000  // Gate stays open for 3 seconds

const char ssid[] = "NotParsaSn";
const char pass[] = "Parsa138282";

// MQTT Configuration
#define MQTT_BROKER "api.smartparking.niflheimsdevs.ir"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "SmartParking_ESP32"

#define ENTRANCE_REQUEST_TOPIC "parking/entrance"
#define ENTRANCE_RESPONSE_TOPIC "parking/entrance/response"
#define EXIT_REQUEST_TOPIC "parking/exit"
#define EXIT_RESPONSE_TOPIC "parking/exit/response"

#define SPACE_TOPIC "parking/space"

#define GATE_CONTROL_TOPIC "parking/gate/control"

#endif
