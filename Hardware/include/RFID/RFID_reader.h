#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Arduino_JSON.h>


// Legacy function (keeping for compatibility)
void setupRFID(int ssPin, int rstPin);

// Dual reader functions
void setupDualRFID();

// Read RFID card and return UID (returns true if card detected)
bool readRFIDEntry(String &cardUID);
bool readRFIDExit(String &cardUID);

// Legacy function (keeping for compatibility - now just reads card)
bool isAuthorizedCard(String &cardUID);
bool isAuthorizedCardEntry(String &cardUID);
bool isAuthorizedCardExit(String &cardUID);

// Helper functions
void printUID(byte *uid, byte size);
void checkReader(MFRC522 &reader, const char* name);

#endif