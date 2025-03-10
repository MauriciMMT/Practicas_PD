#include <Arduino.h>


// Declarar el prototipo de la función anotherTask
void anotherTask(void * parameter);


void setup() {
 // Iniciar la comunicación serial
 Serial.begin(115200);


 // Crear una nueva tarea
 xTaskCreate(
   anotherTask,    // Función de la tarea
   "another Task", // Nombre de la tarea
   1024,           // Tamaño de la pila (1 KB)
   NULL,           // Parámetro de la tarea
   2,              // Prioridad de la tarea (mayor que la tarea principal)
   NULL            // Manejador de la tarea
 );
}


void loop() {
 // Esta es la tarea principal (loop)
 Serial.println("this is ESP32 Task");
 delay(1000); // Usar delay() aquí está bien, ya que es el bucle principal
}


void anotherTask(void * parameter) {
 // Bucle infinito para la tarea adicional
 for (;;) {
   Serial.println("this is another Task");
   vTaskDelay(1000 / portTICK_PERIOD_MS); // Usar vTaskDelay() en tareas de FreeRTOS
 }
 // vTaskDelete(NULL); // Esto no es necesario y nunca se ejecutará
}

