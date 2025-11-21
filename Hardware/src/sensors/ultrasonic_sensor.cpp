#include "sensors/ultrasonic_sensor.h"

// Legacy single sensor variables
int trigPin_;
int echoPin_;

// Multi-sensor arrays for parking system
int trigPins[PARKING_SPACES];
int echoPins[PARKING_SPACES];

// Pin configuration arrays
const int ULTRASONIC_TRIG_PINS[PARKING_SPACES] = {
    ULTRASONIC_TRIG_PIN_0, ULTRASONIC_TRIG_PIN_1, ULTRASONIC_TRIG_PIN_2, ULTRASONIC_TRIG_PIN_3,
    ULTRASONIC_TRIG_PIN_4, ULTRASONIC_TRIG_PIN_5, ULTRASONIC_TRIG_PIN_6
};

const int ULTRASONIC_ECHO_PINS[PARKING_SPACES] = {
    ULTRASONIC_ECHO_PIN_0, ULTRASONIC_ECHO_PIN_1, ULTRASONIC_ECHO_PIN_2, ULTRASONIC_ECHO_PIN_3,
    ULTRASONIC_ECHO_PIN_4, ULTRASONIC_ECHO_PIN_5, ULTRASONIC_ECHO_PIN_6
};

// Legacy single sensor functions
void setupUltrasonic(int trigPin, int echoPin) {
    trigPin_ = trigPin;
    echoPin_ = echoPin;
    pinMode(trigPin_, OUTPUT);
    pinMode(echoPin_, INPUT);
}

// float getDistance() {
//     digitalWrite(trigPin_, LOW);
//     delayMicroseconds(2);
//     digitalWrite(trigPin_, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(trigPin_, LOW);

//     long duration = pulseIn(echoPin_, HIGH);
//     float distance = duration * 0.034 / 2;
//     return distance;
// }

// Multi-sensor functions for parking system
void setupAllUltrasonicSensors() {
    for (int i = 0; i < PARKING_SPACES; i++) {
        trigPins[i] = ULTRASONIC_TRIG_PINS[i];
        echoPins[i] = ULTRASONIC_ECHO_PINS[i];
        pinMode(trigPins[i], OUTPUT);
        pinMode(echoPins[i], INPUT);
        Serial.println("Ultrasonic sensor " + String(i) + " initialized on pins " + 
                      String(trigPins[i]) + " (trigger), " + String(echoPins[i]) + " (echo)");
    }
    Serial.println("All " + String(PARKING_SPACES) + " ultrasonic sensors initialized");
}

float getDistance(int sensorIndex) {
    if (sensorIndex < 0 || sensorIndex >= PARKING_SPACES) {
        return -1;
    }
    
    digitalWrite(trigPins[sensorIndex], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[sensorIndex], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[sensorIndex], LOW);

    long duration = pulseIn(echoPins[sensorIndex], HIGH);
    float distance = duration * 0.034 / 2;
    return distance;
}

bool isParkingSpaceOccupied(int spaceIndex) {
    if (spaceIndex < 0 || spaceIndex >= PARKING_SPACES) {
        return false;
    }
    
    float distance = getDistance(spaceIndex);
    return (distance > 0 && distance < PARKING_THRESHOLD);
}

bool areAllParkingSpacesOccupied() {
    for (int i = 0; i < PARKING_SPACES; i++) {
        if (!isParkingSpaceOccupied(i)) {
            return false;
        }
    }
    return true;
}

int getAvailableParkingSpaces() {
    int availableCount = 0;
    for (int i = 0; i < PARKING_SPACES; i++) {
        if (!isParkingSpaceOccupied(i)) {
            availableCount++;
        }
    }
    return availableCount;
}
