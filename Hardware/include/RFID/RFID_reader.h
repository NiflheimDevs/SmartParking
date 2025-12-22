#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Arduino_JSON.h>
#include "config.h"
#include "mqtt_client.h"

// Dual reader functions
void setupDualRFID();

// Read RFID card and return UID (returns true if card detected)
bool readRFIDEntry(String &cardUID);
bool readRFIDExit(String &cardUID);

// Helper functions
void checkReader(MFRC522 &reader, const char* name);

#endif