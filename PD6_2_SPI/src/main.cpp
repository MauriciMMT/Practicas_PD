#include <SPI.h>         // Incluye la librería para la comunicación SPI
#include <SD.h>          // Incluye la librería para manejar la tarjeta SD

// Pines para ESP32-S3
#define SD_CS   10       // Pin de selección de chip (CS) para la SD
#define SD_MOSI 11       // Pin MOSI (Master Out Slave In)
#define SD_MISO 13       // Pin MISO (Master In Slave Out)
#define SD_SCK  12       // Pin SCK (Serial Clock)

void listFiles();        // Declaración anticipada de la función para listar archivos

void setup() {
  Serial.begin(115200);        // Inicializa la comunicación serie a 115200 baudios
  while(!Serial);              // Espera a que el puerto serie esté listo (útil en algunos entornos de depuración)
  
  Serial.println("\nIniciando sistema..."); // Mensaje de inicio

  // Inicializar SPI con pines manuales
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI); // Inicializa el bus SPI con los pines especificados
  pinMode(SD_CS, OUTPUT);              // Configura el pin CS como salida

  Serial.println("Configurando tarjeta SD..."); // Mensaje de configuración

  // Intentar inicializar con tiempo de espera
  int intentos = 0;                    // Variable para contar los intentos de inicialización
  while(!SD.begin(SD_CS)){             // Intenta inicializar la SD en el pin CS
    Serial.println("Fallo inicialización SD. Intentando nuevamente..."); // Mensaje de error
    delay(1000);                       // Espera 1 segundo antes de reintentar
    if(intentos++ > 5) {               // Si supera 5 intentos
      Serial.println("Error crítico: No se pudo inicializar SD"); // Mensaje de error crítico
      Serial.println("Verifica:");     // Sugerencias de verificación
      Serial.println("1. Conexiones de pines");
      Serial.println("2. Formato FAT32 de la tarjeta");
      Serial.println("3. Voltaje 3.3V en el módulo SD");
      while(1);                        // Detiene la ejecución
    }
  }

  Serial.println("Tarjeta SD inicializada correctamente"); // Mensaje de éxito

  // Listar archivos para verificación
  listFiles();                         // Llama a la función para listar archivos
}

void listFiles() {
  File root = SD.open("/");            // Abre el directorio raíz de la tarjeta SD
  Serial.println("\nContenido de la tarjeta:"); // Mensaje de cabecera

  while(File entry = root.openNextFile()) { // Recorre todos los archivos y carpetas
    Serial.print(entry.name());         // Imprime el nombre del archivo o carpeta
    if(entry.isDirectory()) {           // Si es un directorio
      Serial.println("/");              // Imprime una barra para indicar carpeta
    } else {
      Serial.print("\t");               // Tabulación para alinear el tamaño
      Serial.println(entry.size());     // Imprime el tamaño del archivo
    }
    entry.close();                      // Cierra el archivo o carpeta actual
  }
  root.close();                        // Cierra el directorio raíz
}

void loop() {}                         // Bucle principal vacío, no se ejecuta nada
