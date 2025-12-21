#ifndef PARKING_MONITOR_H
#define PARKING_MONITOR_H

#include <Arduino.h>

// Initialize parking space monitoring
void initParkingMonitor();

// Update parking space monitoring (call from main loop)
// Sends parking space data to MQTT with 0.5 second intervals per space
void updateParkingMonitor();

#endif

