#include "RFID_module.h"


Rfid_unit::Rfid_unit(MFRC522* my_rfid){
    rfid_pntr = my_rfid;
}

boolean Rfid_unit::RFID_scan() {
  boolean readCard = false;
  if ( rfid_pntr->PICC_IsNewCardPresent()) {
    Serial.println(F("\nCard detected, reading..."));
    if ( rfid_pntr->PICC_ReadCardSerial() ) {
      MFRC522::PICC_Type piccType = rfid_pntr->PICC_GetType(rfid_pntr->uid.sak);
      Serial.print(F("PICC type: "));
      Serial.println(rfid_pntr->PICC_GetTypeName(piccType));

      // Check is the PICC of Classic MIFARE type
      if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
          piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
          piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        readCard = false; 
      } else {
        uidString = constructUidString(rfid_pntr->uid.uidByte, rfid_pntr->uid.size);
        Serial.print(F("Scanned PICC's UID: "));
        Serial.println(uidString);
        readCard = true;
      }
    }
  }
  // Halt PICC
  rfid_pntr->PICC_HaltA();
  // Stop encryption on PCD
  rfid_pntr->PCD_StopCrypto1();
  return readCard;
}

String Rfid_unit::constructUidString(byte *buffer, byte bufferSize) {
  String uid = buffer[0] < 0x10 ? "0" : "";
  uid += String(buffer[0], HEX);
  for (byte i = 1; i < bufferSize; i++) {
    uid += buffer[i] < 0x10 ? ":0" : ":";
    uid += String(buffer[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

void Rfid_unit::initializeRFID() {
    Serial.println("Initializing RFID.");
    SPI.begin();              // Init SPI bus
    rfid_pntr->PCD_Init();    // Init MFRC522 
}
