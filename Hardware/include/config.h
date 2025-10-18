#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG true

#define SERVO_PIN 5

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
#define ULTRASONIC_TRIG_PIN_0 12
#define ULTRASONIC_ECHO_PIN_0 14
#define ULTRASONIC_TRIG_PIN_1 13
#define ULTRASONIC_ECHO_PIN_1 15
#define ULTRASONIC_TRIG_PIN_2 16
#define ULTRASONIC_ECHO_PIN_2 17
#define ULTRASONIC_TRIG_PIN_3 18
#define ULTRASONIC_ECHO_PIN_3 19
#define ULTRASONIC_TRIG_PIN_4 22
#define ULTRASONIC_ECHO_PIN_4 23
#define ULTRASONIC_TRIG_PIN_5 25
#define ULTRASONIC_ECHO_PIN_5 26
#define ULTRASONIC_TRIG_PIN_6 27
#define ULTRASONIC_ECHO_PIN_6 32

#define RFID_SS_PIN 21
#define RFID_RST_PIN 47

#define LED_PIN 2
#define LED_COUNT 7
#define LED_BRIGHTNESS 50

#define PARKING_THRESHOLD 17
#define SERVO_OPEN_ANGLE 90
#define SERVO_CLOSED_ANGLE 0
#define SENSOR_READ_INTERVAL 500

#endif
