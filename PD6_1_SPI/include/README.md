Informe de Funcionamiento – Lectura de Etiqueta RFID con MFRC522 (Bus SPI)

El objetivo de esta práctica es comprender el funcionamiento del bus SPI utilizando un lector RFID MFRC522. El programa permite detectar tarjetas RFID, leer su identificador único (UID) y mostrarlo por el puerto serie.

Funcionamiento del Código

1. Inicialización de librerías y pines:
   - Se incluyen las librerías SPI.h y MFRC522.h para la comunicación SPI y el control del lector RFID.
   - Se definen los pines de reset (RST_PIN = 9) y de selección de esclavo (SS_PIN = 10) para el MFRC522.
   - Se crea un objeto MFRC522 para manejar el lector RFID.

2. setup():
   - Se inicia la comunicación serie a 9600 baudios.
   - Se inicializa el bus SPI con los pines SCK=12, MISO=13 y MOSI=11.
   - Se inicializa el lector RFID llamando a mfrc522.PCD_Init().
   - Se imprime el mensaje "¡Listo para leer tarjetas RFID!" por el puerto serie.

3. loop():
   - El programa comprueba continuamente si hay una nueva tarjeta RFID presente y si puede leer su UID.
   - Si detecta una tarjeta, imprime "UID: " seguido del identificador único de la tarjeta en formato hexadecimal, separando cada byte.
   - Después de leer el UID, detiene la comunicación con la tarjeta usando mfrc522.PICC_HaltA().

Salida Esperada por el Puerto Serie

Cuando se acerque una tarjeta RFID al lector, la salida será similar a:

¡Listo para leer tarjetas RFID!
UID:  04  7A  1B  2F
UID:  12  34  56  78

Cada línea "UID: ..." corresponde al identificador único de una tarjeta detectada.

![imatge1](PHOTO-2025-06-14-17-24-10.jpg)
