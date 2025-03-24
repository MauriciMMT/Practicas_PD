#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9   // GPIO9 (Reset del MFRC522)
#define SS_PIN 10   // GPIO10 (SDA/SS del MFRC522)

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin(12, 13, 11);  // SCK=GPIO12, MISO=GPIO13, MOSI=GPIO11
  mfrc522.PCD_Init();
  Serial.println("¡Listo para leer tarjetas RFID!");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    mfrc522.PICC_HaltA();
  }
}