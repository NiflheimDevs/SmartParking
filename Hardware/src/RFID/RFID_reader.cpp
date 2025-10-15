#include "RFID/RFID_reader.h"

MFRC522 mfrc522;

void setupRFID(int ssPin, int rstPin) {
    SPI.begin();
    mfrc522.PCD_Init(ssPin, rstPin);
    Serial.println("RFID reader initialized.");
}

bool isAuthorizedCard(String &cardUID) {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return false;
    }

    cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }

    cardUID.toUpperCase();

    String authorizedCards[] = {"A1B2C3D4", "F1E2D3C4"};
    for (auto &auth : authorizedCards) {
        if (cardUID == auth) {
            Serial.println("✅ Authorized card detected: " + cardUID);
            return true;
        }
    }

    Serial.println("❌ Unauthorized card: " + cardUID);
    return false;
}
