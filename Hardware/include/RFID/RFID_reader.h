#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

void setupRFID(int ssPin, int rstPin);
bool isAuthorizedCard(String &cardUID);

#endif