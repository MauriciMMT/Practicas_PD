¿Qué hace cada parte?
setup(): Inicializa la comunicación serie y crea una tarea adicional (anotherTask) usando FreeRTOS.
loop(): Es la tarea principal de Arduino, imprime cada segundo "this is ESP32 Task".
anotherTask(): Es una tarea FreeRTOS que imprime cada segundo "this is another Task" usando vTaskDelay (que permite multitarea real).

Salida por el Puerto Serie

La salida por el monitor serie será una mezcla de los siguientes mensajes, impresos cada segundo por cada tarea:

this is ESP32 Task
this is another Task
this is ESP32 Task
this is another Task

Ambos mensajes aparecerán repetidamente,  ya que ambas tareas se ejecutan en paralelo (en diferentes núcleos o compartiendo el tiempo de CPU).

Explicación del Funcionamiento

El ESP32 es un microcontrolador de doble núcleo capaz de ejecutar varias tareas en paralelo gracias a FreeRTOS. En este ejemplo:

La función loop() es la tarea principal de Arduino y se ejecuta en uno de los núcleos, imprimiendo un mensaje cada segundo.
La función anotherTask() es una tarea adicional creada explícitamente con FreeRTOS, que también imprime un mensaje cada segundo, pero usando vTaskDelay para ceder el control al planificador de tareas.

Esto demuestra cómo el ESP32 puede ejecutar varias tareas "al mismo tiempo", aprovechando sus dos núcleos y el sistema operativo en tiempo real FreeRTOS.

Respuesta a la Pregunta

¿Por qué es útil usar FreeRTOS y tareas en el ESP32?

Cuando se programa de forma tradicional (sin FreeRTOS), todo el código se ejecuta secuencialmente en un solo núcleo, lo que puede ser ineficiente, especialmente si tienes tareas que pueden bloquear el flujo (por ejemplo, actualizar una pantalla de tinta electrónica que tarda segundos). Si todo se hace en serie, el microcontrolador no puede hacer nada más mientras espera.

Con FreeRTOS, puedes dividir tu programa en tareas independientes que se ejecutan en paralelo o se alternan rápidamente, incluso en diferentes núcleos. Así, puedes leer sensores, actualizar pantallas y comunicarte con la nube al mismo tiempo, sin que una tarea bloquee a las demás. Además, puedes asignar prioridades y decidir en qué núcleo se ejecuta cada tarea, optimizando el rendimiento y la respuesta de tu dispositivo IoT.

Si tienes una pantalla lenta, puedes actualizarla en una tarea aparte, mientras que otra tarea sigue leyendo sensores y enviando datos, logrando un sistema más eficiente y reactivo.
