#include <SPI.h>                // Incluye la librería para la comunicación SPI
#include <MFRC522.h>            // Incluye la librería para el lector RFID MFRC522

#define RST_PIN 9   // GPIO9 (Reset del MFRC522) - Pin de reset del módulo RFID
#define SS_PIN 10   // GPIO10 (SDA/SS del MFRC522) - Pin de selección de esclavo (SS) del módulo RFID

MFRC522 mfrc522(SS_PIN, RST_PIN); // Crea un objeto MFRC522 usando los pines definidos

void setup() {
  Serial.begin(9600);                       // Inicializa la comunicación serie a 9600 baudios
  SPI.begin(12, 13, 11);                    // Inicializa el bus SPI con SCK=GPIO12, MISO=GPIO13, MOSI=GPIO11
  mfrc522.PCD_Init();                       // Inicializa el lector RFID MFRC522
  Serial.println("¡Listo para leer tarjetas RFID!"); // Mensaje de inicio por el puerto serie
}

void loop() {
  // Comprueba si hay una nueva tarjeta presente y si se puede leer su UID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("UID: ");                  // Imprime el texto "UID: " por el puerto serie
    for (byte i = 0; i < mfrc522.uid.size; i++) { // Recorre todos los bytes del UID de la tarjeta
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); // Imprime un espacio y un cero si el byte es menor que 0x10
      Serial.print(mfrc522.uid.uidByte[i], HEX); // Imprime el byte del UID en formato hexadecimal
    }
    Serial.println();                       // Imprime un salto de línea
    mfrc522.PICC_HaltA();                   // Finaliza la comunicación con la tarjeta actual
  }
