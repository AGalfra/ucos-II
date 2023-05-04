# ucos-II

## Sensor de Temperatura ST-1820


Introducción

Este dispositivo desarrollado en el presente informe es un medidor de temperaturas basado en sensores 1-Wire del tipo  capaz de medir hasta 4 sensores simultáneamente (aunque esta limitación está dada solo por configuración del firmware para adaptarla a las opciones del menú).
Tiene además la posibilidad de modificar la escala de medición, es decir, en ºC y ºF.
También es configurable la frecuencia de adquisición de los sensores (3, 5 o 10 segundos).
Para la visualización de las temperaturas se pueden elegir entre 2 modos de operación que serán descritos más adelante.


Descripción
 
Para este proyecto se utilizó la placa de desarrollo Blue Pill que cuenta con un microprocesador ARM Cortex-M3 STM32F103C8T6 de ST Microelectronics en el cual se ha embebido el Kernel v2.9216. 

Además de la placa de desarrollo se han utilizado los siguientes elementos:

-Pantalla OLED SSD1306 128x64 pixeles de 0.96” con comunicación I2C.

-Encoder rotativo, que es utilizado para controlar el dispositivo.

-Sensores de temperatura del tipo 1-Wire como por ej. los DS18B20, DS1820 o DS18S20.

-UART para poder realizar una conexión con la PC (para un menú de diagnóstico). 
