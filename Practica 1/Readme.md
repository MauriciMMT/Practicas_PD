# Práctica 1: Blink

## Introducción

El objetivo de esta práctica es implementar un programa que permita el parpadeo periódico de un LED utilizando un microcontrolador ESP32. Además, se utilizará la salida serie para depurar el programa y enviar mensajes que indiquen el estado del LED (encendido o apagado). 

## Código Proporcionado

El código inicial que se nos proporciona en la práctica consiste en un programa simple que hace parpadear un LED conectado al pin 2 del ESP32. El LED se enciende y apaga con un intervalo de 500 milisegundos.

```cpp
#include <Arduino.h>

#define LED_BUILTIN 2
#define DELAY 500

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  delay(DELAY);
}
```

## 1.Código Adaptado

Para que el código funcione correctamente con nuestra placa, es importante considerar que el LED está conectado al pin 48 y utiliza el formato RGB de Neopixel. Por lo tanto, es necesario adaptar el código para controlar el LED utilizando la librería Adafruit_NeoPixel.

```cpp
#include <Arduino.h>
// Incluye la biblioteca NeoPixel
#include <Adafruit_NeoPixel.h> 

// Pin donde está conectado el NeoPixel
#define LED_PIN 48             

// Número de NeoPixels
#define NUM_PIXELS 1


// Crea un objeto NeoPixel en el PIN 48
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
 // Inicialización del objeto NeoPixel
 pixels.begin();
}

void loop() {
 // Asignación del color Blanco en código RGB para el Led NeoPixel
 pixels.setPixelColor(0, pixels.Color(255, 255, 255));
 // Actualización física del NeoPixel para que muestre el color asignado
 pixels.show(); 
// Espera 500 milisegundos
 delay(500); 
 // Asignación del color Negro en codigo RGB simulando el Apagado
 pixels.setPixelColor(0, pixels.Color(0, 0, 0));
 // Actualización física del NeoPixel para que muestre el color asignado
 pixels.show();
// Espera 500 milisegundos
 delay(500);  
}
```

## 2.Incluir el envío de datos (ON y OFF) al puerto serie.


Se ha añadido la inicialización del puerto serie en la función setup() utilizando Serial.begin(115200). Esto permite establecer una comunicación entre el microcontrolador ESP 32 y el ordenador a una velocidad de transmisión de 115200 baudios.

### platformio.ini
```cpp
monitor_speed = 115200
```

### main.cpp
```cpp
void setup() {
 // Inicialización del objeto NeoPixel
 pixels.begin();
 // Inicialización del puerto serie
 Serial.begin(115200); 
}
```


Cada vez que el LED cambia de estado (encendido o apagado), se envía una línea de texto al puerto serie para indicar su estado actual. Cuando el LED se enciende, se envía el mensaje "ON" y cuando se apaga, se envía el mensaje "OFF". Esto se hace utilizando la función Serial.println() dentro del bucle tal que así:

```cpp

void loop() {
 // Asignación del color Blanco en codigo RGB para el Led NeoPixel
 pixels.setPixelColor(0, pixels.Color(255, 255, 255));
// Actualización física del NeoPixel para que muestre el color asignado
pixels.show(); 
// Envía "ON" al puerto serie
Serial.println("ON"); 
// Espera 1 segundo
delay(1000); 
// Asignación del color Negro en codigo RGB simulando el Apagado
pixels.setPixelColor(0, pixels.Color(0, 0, 0));
// Actualización física del NeoPixel para que muestre el color asignado
pixels.show();
// Envía "OFF" al puerto serie
Serial.println("OFF"); 
// Espera 1 segundo
delay(1000);
}
```

## 3.Actuar directamente sobre los registros de los puertos de entrada y salida.

El acceso directo a los registros permite un control más preciso y eficiente sobre el hardware del microcontrolador, evitando la sobrecarga de las funciones de alto nivel como `digitalWrite()` o `pinMode()`. Esto es especialmente útil en aplicaciones donde el tiempo de respuesta es crítico o donde se necesita un control fino sobre los pines GPIO.

En este caso, `GPIO_OUT_REG` es la dirección del registro de salida GPIO, que controla el estado de los pines GPIO. Para acceder a este registro, se crea un puntero con la línea:

```cpp
uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;
```

Este puntero apunta directamente a la ubicación en memoria del registro. Luego, mediante manipulación de bits, se puede activar o desactivar un pin específico:

```cpp
*gpio_out |= (1 << LED_PIN);  // Activa el bit correspondiente al pin 48
*gpio_out &= ~(1 << LED_PIN); // Desactiva el bit correspondiente al pin 48
```


Además, es importante configurar el pin como salida en el registro de dirección de los GPIO (GPIO_ENABLE_REG). Sin esta configuración, el pin no se comportará como una salida, independientemente de lo que se haga en el registro de salida.

Sin embargo, los NeoPixels no son LEDs comunes., no se pueden controlar simplemente poniendo el pin en HIGH o LOW. Los NeoPixels requieren una señal digital específica con un timing muy preciso para codificar la información de color. La función show() de la librería Adafruit_NeoPixel genera una secuencia de pulsos que representan los bits 0 y 1, y cada bit corresponde a un componente de color (rojo, verde, azul). Por lo tanto, aunque el código manipula correctamente el registro GPIO, no es suficiente para controlar un NeoPixel.


## 4.Sin Delays

Se han eliminado los delays, y se ha modificado el pin de salida al número 2. 

```cpp
#include <Arduino.h>

// Pin de salida configurable
#define LED_PIN 2  

// Punteros a los registros GPIO
uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;
uint32_t *gpio_enable = (uint32_t *)GPIO_ENABLE_REG;

// Variables para el control del tiempo
unsigned long previousMillis = 0;
const long interval = 1000; // Intervalo de 1 segundo
bool ledState = false;

void setup() {
  Serial.begin(115200);
  *gpio_enable |= (1 << LED_PIN);  // Configurar el pin como salida
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState; // Alternar el estado del LED
    
    if (ledState) {
      *gpio_out |= (1 << LED_PIN);
      Serial.println("ON");
    } else {
      *gpio_out &= ~(1 << LED_PIN);
      Serial.println("OFF");
    }
  }
}
```


Luego, se ha medido la máxima frecuencia de apagado/encendido en cuatro casos diferentes utilizando el osciloscopio:

- Con el envío por el puerto serie del mensaje y utilizando las funciones de Arduino
- Con el envío por el puerto serie y accediendo directamente a los registros
- Sin el envío por el puerto serie del mensaje y utilizando las funciones de Arduino
- Sin el envío por el puerto serie y accediendo directamente a los registros


## 1.Diagrama de flujo con NeoPixel y delay()

1. Incluir la librería `Adafruit_NeoPixel`.
2. Definir el pin 48 como conexión del LED.
3. Crear el objeto `Adafruit_NeoPixel`.
4. En `setup()`:
   - Llamar a `pixels.begin()` para inicializar.
5. En `loop()`:
   - Encender el LED (color blanco).
   - Ejecutar `pixels.show()`.
   - Esperar 500 ms con `delay(500)`.
   - Apagar el LED (color negro).
   - Ejecutar `pixels.show()`.
   - Esperar 500 ms con `delay(500)`.

## Diagrama de tiempos con con NeoPixel y delay()

![Texto alternativo](Tiempos1.jpg)

## 2. Diagrama de flujo con salida al puerto serie (ON/OFF)

1. Repetir los pasos de la versión 1.
2. En `setup()`:
   - Añadir `Serial.begin(115200)`.
3. En `loop()`:
   - Encender LED y llamar a `show()`.
   - Enviar `"ON"` al puerto serie.
   - Esperar 1000 ms.
   - Apagar LED y llamar a `show()`.
   - Enviar `"OFF"` al puerto serie.
   - Esperar 1000 ms.

## Diagrama de tiempos con salida al puerto serie (ON/OFF)

![Texto alternativo](Tiempos2.jpg)

## 3. Diagrama de flujo con acceso directo a registros GPIO

1. Definir `LED_PIN` como 48.
2. Crear punteros a `GPIO_OUT_REG` y `GPIO_ENABLE_REG`.
3. En `setup()`:
   - Iniciar `Serial.begin(115200)`.
   - Configurar el pin como salida con `*gpio_enable |= (1 << LED_PIN);`.
4. En `loop()`:
   - Encender el pin con `*gpio_out |= (1 << LED_PIN);`.
   - Enviar `"ON"` al puerto serie.
   - Esperar 1000 ms.
   - Apagar el pin con `*gpio_out &= ~(1 << LED_PIN);`.
   - Enviar `"OFF"` al puerto serie.
   - Esperar 1000 ms.

## Diagrama de tiempos con acceso directo a registros GPIO

![Texto alternativo](Tiempos3.jpg)

## 4. Diagrama de flujo sin delay(), con registros y millis()

1. Definir `LED_PIN` como 2.
2. Crear punteros a `GPIO_OUT_REG` y `GPIO_ENABLE_REG`.
3. Declarar variables globales:
   - `previousMillis`, `interval = 1000`, `ledState`.
4. En `setup()`:
   - Iniciar `Serial.begin(115200)`.
   - Configurar el pin como salida con `*gpio_enable |= (1 << LED_PIN);`.
5. En `loop()`:
   - Leer el tiempo actual con `millis()`.
   - Si ha pasado 1 segundo:
     - Alternar `ledState`.
     - Encender o apagar el pin mediante acceso al registro.
     - Enviar `"ON"` o `"OFF"` al puerto serie.


## ¿En el programa que se ha realizado cuál es el tiempo libre que tiene el procesador?

El "tiempo libre" del procesador en un programa embebido como este se refiere al tiempo durante el cual el procesador no está realizando ninguna tarea activa, es decir, cuando no está ejecutando instrucciones y está esperando (idle). En el caso de esta ultima versión del código, como hemos eliminado el uso de delay() solo hay un componente que defina el período de inactividad donde el procesador no realiza ninguna operación.

El tiempo de espera durante la transmisión de datos por el puerto serie: El uso de Serial.println() también introduce algo de latencia si el puerto serie está ocupado transmitiendo datos. Aunque no es una espera activa como el delay(), la transmisión de datos puede influir en el tiempo total de ejecución.

## Temperatura

Este código lee la temperatura interna del microcontrolador y la envía al puerto serie para ser visualizada en un monitor serie. En el setup(), se inicia la comunicación serie y se espera un segundo para estabilizar la conexión. Luego, en el loop(), el microcontrolador lee la temperatura utilizando la función temperatureRead() y la imprime en la pantalla cada segundo. El proceso se repite continuamente, proporcionando actualizaciones periódicas de la temperatura interna.

```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  float temperature = temperatureRead();
  Serial.print("Temperatura interna: ");
  Serial.print(temperature);
  Serial.println(" °C");
  delay(1000);
}
```
