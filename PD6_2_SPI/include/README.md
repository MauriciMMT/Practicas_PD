Informe de Funcionamiento – Lectura y Listado de Archivos en Tarjeta SD usando SPI (ESP32-S3)

El objetivo de este programa es inicializar el bus SPI en un ESP32-S3 para comunicarse con una tarjeta SD, comprobar su correcta inicialización y listar el contenido de la tarjeta (archivos y carpetas) por el puerto serie. Esto permite verificar tanto la conexión física como el funcionamiento del bus SPI y la tarjeta SD.

Funcionamiento del Código

1. Inicialización de la comunicación serie:
   El programa comienza inicializando la comunicación serie a 115200 baudios para mostrar mensajes de estado y depuración.

2. Configuración manual del bus SPI:
   Se inicializa el bus SPI especificando manualmente los pines SCK, MISO y MOSI, y se configura el pin CS (chip select) como salida.

3. Inicialización de la tarjeta SD:
   El programa intenta inicializar la tarjeta SD en el pin CS definido. Si falla, lo intenta hasta 5 veces, mostrando mensajes de error y sugerencias de verificación (conexiones, formato, voltaje). Si tras 5 intentos no se logra, el programa se detiene.

4. Listado de archivos:
   Si la tarjeta SD se inicializa correctamente, se llama a la función listFiles() que recorre y muestra por el puerto serie todos los archivos y carpetas en la raíz de la tarjeta, mostrando también el tamaño de cada archivo.

5. Bucle principal:
   El bucle loop() está vacío, ya que toda la funcionalidad se realiza en el setup().

Salida:

- Si la tarjeta SD se inicializa correctamente:

  Iniciando sistema...
  Configurando tarjeta SD...
  Tarjeta SD inicializada correctamente

  Contenido de la tarjeta:
  archivo1.txt    1234
  carpeta/
  imagen.jpg      56789
  
- Si la tarjeta SD no se inicializa:
  
  Iniciando sistema...
  Configurando tarjeta SD...
  Fallo inicialización SD. Intentando nuevamente...

  Error crítico: No se pudo inicializar SD
  Verifica:
  1. Conexiones de pines
  2. Formato FAT32 de la tarjeta
  3. Voltaje 3.3V en el módulo SD
