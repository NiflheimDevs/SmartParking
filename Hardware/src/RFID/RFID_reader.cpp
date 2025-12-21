#include "RFID/RFID_reader.h"
#include "config.h"
#include "mqtt_client.h"

MFRC522 mfrc522;

// Dual reader instances
MFRC522 rfidEntry(RFID_ENTRY_SS_PIN, RFID_ENTRY_RST_PIN);
MFRC522 rfidExit(RFID_EXIT_SS_PIN, RFID_EXIT_RST_PIN);

// Legacy function (keeping for compatibility)
void setupRFID(int ssPin, int rstPin) {
    SPI.begin();
    mfrc522.PCD_Init(ssPin, rstPin);
    Serial.println("RFID reader initialized.");
}

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

void printUID(byte *uid, byte size) {
    for (byte i = 0; i < size; i++) {
        if (uid[i] < 0x10) Serial.print("0");
        Serial.print(uid[i], HEX);
    }
    Serial.println();
}

// Legacy functions (keeping for compatibility - now just reads card)
bool isAuthorizedCard(String &cardUID) {
    return readRFIDEntry(cardUID);
}

bool isAuthorizedCardEntry(String &cardUID) {
    return readRFIDEntry(cardUID);
}

bool isAuthorizedCardExit(String &cardUID) {
    return readRFIDExit(cardUID);
}
