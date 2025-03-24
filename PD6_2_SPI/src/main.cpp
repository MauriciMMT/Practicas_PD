#include <SPI.h>
#include <SD.h>

// Pines para ESP32-S3
#define SD_CS   10
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK  12
void listFiles();

void setup() {
  Serial.begin(115200);
  while(!Serial); // Solo para depuración
  
  Serial.println("\nIniciando sistema...");
  
  // Inicializar SPI con pines manuales
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  pinMode(SD_CS, OUTPUT);
  
  Serial.println("Configurando tarjeta SD...");
  
  // Intentar inicializar con tiempo de espera
  int intentos = 0;
  while(!SD.begin(SD_CS)){
    Serial.println("Fallo inicialización SD. Intentando nuevamente...");
    delay(1000);
    if(intentos++ > 5) {
      Serial.println("Error crítico: No se pudo inicializar SD");
      Serial.println("Verifica:");
      Serial.println("1. Conexiones de pines");
      Serial.println("2. Formato FAT32 de la tarjeta");
      Serial.println("3. Voltaje 3.3V en el módulo SD");
      while(1); // Detener ejecución
    }
  }
  
  Serial.println("Tarjeta SD inicializada correctamente");
  
  // Listar archivos para verificación
  listFiles();
}

void listFiles() {
  File root = SD.open("/");
  Serial.println("\nContenido de la tarjeta:");
  
  while(File entry = root.openNextFile()) {
    Serial.print(entry.name());
    if(entry.isDirectory()) {
      Serial.println("/");
    } else {
      Serial.print("\t");
      Serial.println(entry.size());
    }
    entry.close();
  }
  root.close();
}

void loop() {}