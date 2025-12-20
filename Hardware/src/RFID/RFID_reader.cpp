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


void readRFIDEntry() {
    if (rfidEntry.PICC_IsNewCardPresent() && rfidEntry.PICC_ReadCardSerial()) {
        JSONVar jsonObj;
        jsonObj["rfid : "] = String(rfidEntry.uid.uidByte,HEX);
        String jsonString = JSON.stringify(jsonObj);
        client.publish("parking/entrance", jsonString.c_str());
        rfidEntry.PICC_HaltA();
    }
    delay(200);
}

