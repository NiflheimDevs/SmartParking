#include "RFID/RFID_reader.h"

// Dual reader instances
MFRC522 rfidEntry(RFID_ENTRY_SS_PIN, RFID_ENTRY_RST_PIN);
MFRC522 rfidExit(RFID_EXIT_SS_PIN, RFID_EXIT_RST_PIN);

// Setup dual RFID readers with custom SPI pins
void setupDualRFID() {
    Serial.println("\n🔍 Initializing SPI bus and RC522 modules...");

    // Initialize SPI bus with custom pins
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);

    // Initialize both readers
    rfidEntry.PCD_Init();
    rfidExit.PCD_Init();
    
    // Check versions
    checkReader(rfidEntry, "Entry RFID Reader");
    checkReader(rfidExit, "Exit RFID Reader");

    Serial.println("📡 Both RFID Readers Ready.\n");
}


bool readRFIDEntry(String &cardUID) {
    if (rfidEntry.PICC_IsNewCardPresent() && rfidEntry.PICC_ReadCardSerial()) {
        // Convert UID to string
        cardUID = "";
        for (byte i = 0; i < rfidEntry.uid.size; i++) {
            if (rfidEntry.uid.uidByte[i] < 0x10) cardUID += "0";
            cardUID += String(rfidEntry.uid.uidByte[i], HEX);
        }
        cardUID.toUpperCase();
        rfidEntry.PICC_HaltA();
        return true;
    }
    return false;
}

bool readRFIDExit(String &cardUID) {
    if (rfidExit.PICC_IsNewCardPresent() && rfidExit.PICC_ReadCardSerial()) {
        // Convert UID to string
        cardUID = "";
        for (byte i = 0; i < rfidExit.uid.size; i++) {
            if (rfidExit.uid.uidByte[i] < 0x10) cardUID += "0";
            cardUID += String(rfidExit.uid.uidByte[i], HEX);
        }
        cardUID.toUpperCase();
        rfidExit.PICC_HaltA();
        return true;
    }
    return false;
}

void checkReader(MFRC522 &reader, const char* name) {
    byte version = reader.PCD_ReadRegister(reader.VersionReg);
    if (version == 0x00 || version == 0xFF) {
        Serial.println("⚠️ " + String(name) + " not found or communication failure");
    } else {
        Serial.println("✅ " + String(name) + " detected (Version: 0x" + String(version, HEX) + ")");
    }
}
