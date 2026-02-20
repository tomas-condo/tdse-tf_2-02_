
<img width="762" height="292" alt="image" src="https://github.com/user-attachments/assets/f4169b6a-832d-42a4-a46d-fa6cc8cc8f36" />

# **Juego Interactivo "Simón dice"  INFORME FINAL** 

**Autores: Tomás Ignacio Condo, Camila Trinidad Morasso y Francisco Javier Briones**

**Padrones: 111.457, 110.790, 110.058**

**Fecha: 2do cuatrimestre 2025**

<div align="justify">
  
### *Resumen*
En este trabajo se realiza el diseño e implementación del juego "Simon Dice" basado en la arquitectura ARM Cortex-M3. Este sistema tiene por objetivo proporcionar una plataforma de entretenimiento y entrenamiento cognitivo, permitiendo al usuario ejercitar su memoria a corto plazo mediante la reproducción de secuencias aleatorias de luces. El sistema cuenta con características avanzadas como regulación automática de brillo ambiental mediante sensores LDR, almacenamiento no volátil de puntajes máximos en memoria EEPROM y una interfaz de usuario rica visualizada en un display LCD de 20x4 caracteres.

La implementación del mismo se realizó bajo el paradigma Bare Metal (sin sistema operativo), utilizando lenguaje C y manipulando directamente los registros de la placa STM32 Nucleo-F103RB. El diseño de software se estructura estrictamente mediante Máquinas de Estados Finitos (FSM) jerárquicas y modularizadas, garantizando un código robusto, escalable y mantenible.

En esta memoria se presenta la motivación del proyecto, el diseño del hardware y firmware, y los resultados de los ensayos funcionales.
  
# Índice 
1. [Introducción general](#introducción-general)
     - [1.1 Objetivos](#11-objetivos)
     - [1.2 Análisis de mercado](#12-análisis-de-mercado)
2. [Introducción específica](#introducción-específica)
    - [2.1 Requisitos del proyecto](#21-requisitos-del-proyecto)
    - [2.2 Casos de uso](#22-casos-de-uso)
        - [2.2.1 Caso de uso 1: El usuario juega una partida en modo clásico](#221-caso-de-uso-1-el-usuario-juega-una-partida-en-modo-clásico)
        - [2.2.2 Caso de uso 2: El usuario cambia la dificultad del juego](#222-caso-de-uso-2-el-usuario-cambia-la-dificultad-del-juego)
        - [2.2.3 Caso de uso 3: El usuario consulta los puntajes máximos](#223-caso-de-uso-3-el-usuario-consulta-los-puntajes-máximos)
    - [2.3 Elementos de hardware](#22-elementos-de-hardware)
        - [2.3.1 Buttons](#221-buttons)
        - [2.3.2 Leds (Diodos Emisores de Luz)](#222-leds-(diodos-emisores-de-luz))
        - [2.3.3 LDR sensor analógico](#223-ldr-sensor-analógico)
        - [2.3.4 Display LCD](#224-display-lcd)
        - [2.3.5 Memoria E2PROM externa](#225-memoria-e2prom-externa)
        - [2.3.6 Placa de desarrollo](#226-placa-de-desarrollo)
3. [Diseño de implementación](3-diseño-de-implementación)
    - [3.1 Esquema eléctrico y conexión de placas](#31-esquema-eléctrico-y-conexion-de-placas)
    - [3.2 Descripción del esquema eléctrico](#32-descripción-del-esquema-eléctrico)
    - [3.3 Descripción del comportamiento](#33-descripción-del-comportamiento)
    - [3.4 Firmware del Simon Says](#34-firmware-del-simon-says)
        - [3.4.1 Task actuator](#341-task-actuator)
        - [3.4.2 Task sensor](#342-task-sensor)
        - [3.4.3 Task ADC](#343-task-adc)
        - [3.4.4 Task PWM](#344-task-pwm)
        - [3.4.5 Task gameplay](#345-task-gameplay)
        - [3.4.6 Task storage](#346-task-storage)
        - [3.4.7 Task I2C](#347-task-i2c)
        - [3.4.8 Task display](#348-task-display)
        - [3.4.9 Task menu](#349-task-menu)
4. [Ensayos y resultados](#4-ensayos-y-resultados)
    - [4.1 Medición y análisis de consumo](#41-medición-y-análisis-de-consumo)
    - [4.2 Medición y análisis de tiempos de ejecución (WCET)](#42-medición-y-análisis-de-tiempos-de-ejecución-(WCET))
    - [4.3 Cálculo del factor de uso (U) de la CPU](#43-cálculo-del-factor-de-uso-u-de-la-cpu)
    - [4.4 Cumplimiento de requisitos](#44-cumplimiento-de-requisitos)
    - [4.5 Reporte de uso](#45-reporte-de-uso)
    - [4.6 Prueba de integración](#46-prueba-de-integración)
5. [Conclusiones](#5-conclusiones)
    - [5.1 Resultados obtenidos](#51-resultados-obtenidos)
    - [5.2 Próximos pasos](#52-próximos-pasos)
6. [Bibliografía](#6-bibliografía)


# **Introducción general** 
## 1.1 Objetivos

El objetivo del proyecto es repensar el juego “Simon dice”. Al ser un juego que desafía la memoria visual, la concentración y los reflejos, mejorando la capacidad de atención y la resolución de problemas creemos que puede ser llevado al ámbito de la rehabilitación cognitiva, la prevención del deterioro cognitivo y en desarrollo infantil para estimular la neuroplasticidad ya que, al requerir un enfoque visual constante mejora la atención sostenida, la capacidad de retener información a corto plazo y la memoria episódica y semántica.

## 1.2 Análisis de mercado
En el mercado existen diversos productos relacionados con juegos de memoria y reflejos, desde el [clásico “Simon” de Hasbro](https://instructions.hasbro.com/es-cl/instruction/simon-game), hasta juguetes electrónicos genéricos y aplicaciones móviles que imitan el mismo concepto. 

El juguete clásico no guarda el historial de las diversas partidas y, al tener una dificultad fija que se acelera demasiado rápido para un paciente en rehabilitación o un niño con dificultades de aprendizaje vimos una oportunidad de mejora y de inclusión. 

Se tomó como referente [Lumosity](https://app.lumosity.com/es/landing), [CogniFit](https://www.cognifit.com/ar?srsltid=AfmBOooEsyYxj9pAMaxhdNQi1WLkWoFHKCuE0GOEz-8ze5Ma6s48iJ-7) y [Peak](https://www.peak.net/), aplicaciones de ejercicios mentales en tablet/celular para analizar las diversas opciones que el mercado ofrece. Estás tienen la desventaja de la barrera tecnológica, a los adultos mayores les cuesta usar pantallas táctiles, por lo que, un botón físico grande e iluminado es más intuitivo. Además, al ser utilizadas en pantallas táctiles no trabajan la motricidad fina ni la propiocepción de la misma forma que presionar un botón mecánico real. 

El presente proyecto recupera ese aspecto físico y educativo, brindando un entorno ideal para el desarrollo de estimulación neurocognitiva que, a diferencia de los juguetes comerciales, que carecen de registros de progreso, este desarrollo propone una solución de bajo costo brindando esta posibilidad, sumado a la personalización de modos de juego, visibilidad y diversos parámetros como tiempo de encendido de luz a través de este prototipo basado en STM32. 

# Introducción específica
## 2.1 Requisitos del proyecto

En la Tabla 2.1 se detallan los principales requisitos funcionales del sistema:
| Grupo | ID | Descripción |
| :---- | :---- | :---- |
| Juego | 1.1 | El sistema generará una secuencia de LEDs pseudoaleatoria de longitud creciente. |
|  | 1.2 | El sistema permitirá al jugador repetir la secuencia mediante cuatro pulsadores asociados a los cuatro LEDs. |
|  | 1.3 | El sistema comparará la secuencia ingresada por el jugador con la secuencia objetivo y determinará si es correcta. |
|  | 1.4 | En caso de acierto, el sistema incrementará la longitud de la secuencia y avanzará al siguiente nivel. |
|  | 1.5 | En caso de error, el sistema finalizará la ronda y mostrará el resultado al jugador. |
|  | 1.6 | En modo Normal, al iniciar cada nivel se reproducirá la secuencia completa acumulada. |
|  | 1.7 | En modo Difícil, al iniciar cada nivel solo se reproducirá el nuevo color agregado a la secuencia. |
| Interfaz luminosa | 2.1 | Cada LED estará asociado a un color fijo y a un pulsador específico. |
|  | 2.2 | Durante la reproducción de la secuencia, el LED correspondiente se encenderá de forma claramente distinguible. |
|  | 2.3 | Al presionar un pulsador, el LED asociado se encenderá mientras dure la pulsación. |
|  | 2.4 | El sistema implementará antirrebote por software para los cuatro pulsadores. |
|  | 2.5 | El sistema deberá registrar pulsaciones rápidas sin perder eventos. |
|  | 2.6 | El brillo de los LEDs se ajustará automáticamente según el valor leído en el sensor LDR. |
| Sensor LDR | 3.1 | El sistema contará con un sensor de luz LDR conectado a una entrada analógica del STM32. |
|  | 3.2 | El sistema leerá periódicamente el valor de la LDR mediante el ADC. |
|  | 3.3 | El sistema ajustará el ciclo de trabajo PWM de los LEDs en función de la luminosidad ambiente. |
| Pantalla LCD | 4.1 | El sistema contará con una pantalla LCD para mostrar información de estado. |
|  | 4.2 | Al encender el sistema, el LCD mostrará una pantalla de bienvenida durante unos segundos. |
|  | 4.3 | Luego de la bienvenida, el LCD mostrará una pantalla de selección de dificultad (Normal / Difícil). |
|  | 4.4 | Durante el juego, el LCD mostrará el puntaje actual del jugador. |
|  | 4.5 | Al apagar o finalizar el juego, el LCD mostrará una pantalla de despedida. |
| Menú con botones | 5.1 | Todos los menús se manejarán exclusivamente con los cuatro botones del juego. |
|  | 5.2 | Al menos un botón permitirá avanzar entre opciones y otro confirmará la selección. |
|  | 5.3 | El sistema indicará en pantalla las opciones seleccionadas y confirmadas. |
| Audio | 6.1 | El sistema contará con un buzzer para producir señales sonoras. |
|  | 6.2 | Cada color del juego tendrá asignado un tono característico reproducido por el buzzer. |
|  | 6.3 | El sistema reproducirá una melodía corta al superar un nivel. |
|  | 6.4 | El sistema reproducirá una secuencia de error cuando el jugador falle la secuencia. |
|  | 6.5 | El sistema podrá emitir sonidos breves al navegar por el menú. |
| Modos de juego y dificultad | 7.1 | El sistema contará al menos con dos niveles de dificultad: Normal y Difícil. |
|  | 7.2 | La dificultad podrá afectar la velocidad de reproducción de la secuencia y/o el tiempo de respuesta permitido. |
|  | 7.3 | En Normal se reproducirá la secuencia completa en cada nivel; en Difícil, solo el nuevo color agregado. |
| Persistencia y estadísticas (EEPROM) | 8.1 | El sistema almacenará el puntaje máximo alcanzado en memoria EEPROM externa. |
|  | 8.2 | El sistema permitirá leer y mostrar el puntaje máximo guardado al inicio o desde un menú de estadísticas. |
|  | 8.3 | El sistema permitirá reiniciar el récord (borrar el puntaje máximo guardado) desde el menú. |
|  | 8.4 | La EEPROM podrá usarse para almacenar configuraciones de dificultad u otros parámetros del juego. |
| Seguridad y robustez | 9.1 | El sistema deberá iniciar siempre en un estado seguro, con LEDs y buzzer apagados hasta que el usuario interactúe. |
|  | 9.2 | El sistema organizará su lógica en una máquina de estados para evitar bloqueos y comportamientos impredecibles. |
|  | 9.3 | El sistema deberá indicar mediante mensajes en la pantalla y señales sonoras si ocurre un error interno o condición inesperada. |

<p align="center"><em>Tabla 2.1: Requisitos del proyecto.</em></p>

# 2.2 Casos de uso

A continuación en las tablas 2.2, 2.3 y 2.4 se especificarán los diferentes casos de uso.

## 2.2.1 Caso de uso 1: El usuario juega una partida en modo clásico

| Elemento | Definición |
| :---- | :---- |
| Disparador | El jugador quiere iniciar una nueva partida (Normal o Difícil). |
| Precondiciones | El sistema está encendido. Se ha mostrado la pantalla de bienvenida. El jugador ha seleccionado la opción "Jugar" propuesta por el menú utilizando los botones del juego. Todos los LEDs se encuentran apagados. |
| Flujo principal | El jugador navega el menú utilizando los botones y selecciona la dificultad. El sistema genera una secuencia pseudoaleatoria inicial de un solo LED y la reproduce con luz (según el modo: secuencia completa o solo el nuevo color). El jugador repite la secuencia utilizando los cuatro pulsadores; por cada pulsación correcta se enciende el LED correspondiente. Si el jugador ingresa correctamente toda la secuencia, el sistema incrementa la longitud en un elemento, actualiza el nivel y el puntaje y muestra el nuevo puntaje en la pantalla LCD. Este ciclo se repite hasta que el jugador comete un error, donde el sistema muestra un mensaje de “Game Over” en el LCD y finaliza la partida mostrando el puntaje final.
<p align="center"><em>Tabla 2.2: Caso de uso 1: El usuario juega una partida</em></p>

 ## 2.2.2 Caso de uso 2: El usuario cambia la dificultad del juego

| Elemento | Definición |
| :---- | :---- |
| Disparador | El jugador quiere cambiar el nivel de dificultad del juego (Normal / Difícil). |
| Precondiciones | El sistema está encendido. No hay una partida en curso. El juego se encuentra en el menú principal o en el menú de configuración. |
| Flujo principal | El jugador accede a la pantalla de selección de dificultad utilizando los botones del juego. En el LCD se muestran las opciones “Normal” y “Difícil”, avanzando con el selector en dificil pulsando "Enter". El sistema almacena la nueva dificultad seleccionada en el struct de gameplay, actualiza los parámetros internos (forma de reproducir la secuencia, tiempos, etc.) y vuelve al menú principal mostrando la dificultad activa. |

<p align="center"><em>Tabla 2.3: Caso de uso 2: El usuario cambia la dificultad del juego</em></p>

 ## 2.2.3 Caso de uso 3: El usuario consulta los puntajes máximos

| Elemento | Definición |
| :---- | :---- |
| Disparador | El jugador quiere consultar los puntajes máximos almacenados en la memoria EEPROM. |
| Precondiciones | El sistema está encendido. No hay una partida en curso. El juego se encuentra en el menú principal. La EEPROM ha sido inicializada correctamente. |
| Flujo principal | El jugador navega hasta el menú de “Puntajes” utilizando los mismos botones del juego. El sistema lee de la EEPROM los 3 puntajes máximos almacenados y los muestra en la pantalla LCD. 

<p align="center"><em>Tabla 2.4: Caso de uso 3: El usuario consulta el puntaje máximo</em></p>

## 2.3 Elementos de hardware:

### 2.3.1 Buttons

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utilizaron cuatro pulsadores <strong>Touch Switch de 7,5mm</strong> como dispositivos de entrada principal.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Navegación por el menú.</li>
        <li>Ingreso de secuencia de colores (Gameplay).</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utilizaron mecanismos de <em>debounce por software</em> para filtrar el ruido mecánico y garantizar una lectura limpia de la señal.</p>
    </td>
    <td width="40%" align="center">
      <img src="https://github.com/user-attachments/assets/6a23cbbe-e4e6-4d44-b88c-07d74bc2121b" width="250">
      <br><br>
      <em>Imagen 2.1: Pulsador de base grande utilizado.</em>
    </td>
  </tr>
</table>

### 2.3.2 Leds (Diodos Emisores de Luz)
<table>
  <tr>
    <td width="60%" valign="top">
      <p>Como actuadores visuales principales, se emplearon cuatro LEDs de alto brillo en colores rojo, verde, azul y amarillo. Presentan la secuencia que el usuario debe memorizar y replicar.</p>
      <p>Cada LED está controlado por un canal PWM, permitiendo no solo el encendido y apagado (ON/OFF), sino también la regulación de la intensidad lumínica según las condiciones ambientales.</p>
    </td>
    <td width="40%" align="center">
      <img src="https://github.com/user-attachments/assets/a923bc55-ecc7-4221-8a0b-ecd35c741caf" width="200">
      <br><br>
      <em>Imagen 2.2: Diodos LEDs utilizados.</em>
    </td>
  </tr>
</table>

### 2.3.3 LDR (Sensor analógico)
<table>
  <tr>
    <td width="60%" valign="top">
      <p>Para dotar al sistema de capacidad adaptativa, se integró un sensor analógico LDR (Light Dependent Resistor o Fotorresistencia). Este sensor mide la intensidad de la luz ambiental en tiempo real al prender el juego.</p>
    </td>
    <td width="40%" align="center">
      <img src="https://github.com/user-attachments/assets/0886f18a-fa2f-44a9-8f7c-94493d16a883" width="176">
      <br><br>
      <em>Imagen 2.3: Sensor LDR utilizado.</em> 
    </td>
  </tr>
</table>

### 2.3.4 Display LCD
<table>
  <tr>
    <td width="60%" valign="top">
      <p>Para la visualización de datos alfanuméricos, se utilizó un módulo LCD (Liquid Crystal Display). Constituye la interfaz principal de comunicación con el usuario (HMI), mostrando el menú de bienvenida, instrucciones, puntaje actual, entre otras pantallas.</p>
    </td>
    <td width="40%" align="center">
      <img src="https://github.com/user-attachments/assets/925e7613-ac74-4276-904e-c581b1f05a5e" width="200">
      <br><br>
      <em>Imagen 2.4: Display LCD utilizado.</em>
    </td>
  </tr>
</table>

### 2.3.5 Memoria E2PROM externa
<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se incorporó un módulo de memoria no volátil externa tipo EEPROM (familia AT24Cxxx) conectada a través del bus I2C.</p>
      <p>Se utiliza para almacenar la tabla de los tres mejores puntajes históricos, ya que conserva la información cuando el dispositivo se desconecta de la alimentación. Esto es vital para el enfoque de rehabilitación cognitiva del proyecto, ya que permite llevar un registro del progreso y desempeño del usuario a lo largo de las sesiones.</p>
    </td>
    <td width="40%" align="center">
      <img src="https://github.com/user-attachments/assets/65b4cff7-fc20-4808-9c69-925c4ad1f505" width="200">
      <br><br>
      <em>Imagen 2.5: Memoria externa utilizada.</em>
    </td>
  </tr>
</table>

### 2.3.6 Placa de desarrollo (Microcontrolador)
<table>
  <tr>
    <td width="55%" valign="top">
      <p>Como unidad central de procesamiento se utilizó la placa de desarrollo <strong>NUCLEO-F103RB</strong>.</p>
      <p>Esta placa incorpora un microcontrolador STM32 (ARM Cortex-M3) de 32 bits, encargado de gestionar toda la lógica del juego (Máquina de Estados), el manejo de tiempos críticos mediante Timers y la comunicación con todos los periféricos a través de protocolos I2C, ADC y PWM.</p>
    </td>
    <td width="45%" align="center">
      <img src="https://github.com/user-attachments/assets/a657bf6e-783d-4a81-aee4-6a8a7ba27641" width="280">
      <br>
      <em>Imagen 2.6: Placa NUCLEO-F103RBTX utilizada.</em>
    </td>
  </tr>
</table>

# 3. Diseño e implementación 
## 3.1 Esquema eléctrico y conexión de placas

Para la integración física del sistema se soldó a una placa experimental de (15 x 20) cm para garantizar la robustez mecánica y eléctrica del prototipo. El circuito se centra en la placa de desarrollo STM32, la cual gestiona los periféricos mediante las siguientes interfaces:

Interfaz de Entrada (GPIO): En estos pines serán conectadas las teclas ya que no requieren de ningún requisito en especial.

Interfaz de Salida (PWM): Los 4 LEDs de alto brillo se conectan a través de resistencias limitadoras de corriente para proteger los puertos del microcontrolador. Se utilizan canales de Timer para permitir la modulación de brillo (PWM).

Bus de Comunicación (I2C): En dos pines preconfigurados SCL y SDA fueron inicializados y conectados a los pines de datos de la memoria EEPROM.

Sensores Analógicos (ADC): En un pin preconfigurado al seleccionar en el archivo .ioc conectaremos el LDR tal que pueda enviar las muestras obtenidas al inicializar.

Se tiene una noción visual de la configuración de cada pin en la iamgen 3.1:

  <div align="center">
  <img width="440" height="450" alt="image" src="https://github.com/user-attachments/assets/a9a2d5ef-5c2f-4131-9a0c-987458883e0c" />
  <p><em>Imagen 3.1: IOC.</em></p>
</div>

A continuación, en la imagen 3.2 se tienen los diversos componentes conectados a la placa experimental:

  <div align="center">
  <img width="600" height="1000" alt="Gemini_Generated_Image_wrktq6wrktq6wrkt" src="https://github.com/user-attachments/assets/3a13e912-17ae-497b-986e-324b3b6481b0" />
  <p><em>Imagen 3.2: Placa soldada frente.</em></p>
</div>

También, se puede ver en la imagen 3.3 las soldaduras en la placa en su otra cara:

  <div align="center">
  <img width="576" height="1125" alt="Gemini_Generated_Image_ww0mpwww0mpwww0m" src="https://github.com/user-attachments/assets/b5015b90-6761-4323-92ef-d555fa747698" />  
  <p><em>Imagen 3.3: Placa soldada dorso.</em></p>
</div>

El sistema se alimenta por USB de la Nucleo, pudiendo verse en la Imagen 3.4:

  <div align="center">
  <img width="500" height="500" alt="image" src="https://github.com/user-attachments/assets/a2a4f49f-4e85-42ec-a75d-7384a59d4de3" />
  <p><em>Imagen 3.4: Diseño esquema eléctrico.</em></p>
</div>

## 3.2 Descripción del esquema eléctrico 

El circuito integra tres bloques funcionales principales: una etapa de sensado de luz con el sensor LDR de manera analógica, una etapa de entrada de usuario mediante pulsadores y una etapa de visualización de estado mediante diodos LED. Todas las señales están referenciadas a una tierra común, GND.

El LDR visto en la figura 2.3 está configurado junto con una resistencia fija de 10 kΩ. El nodo central del divisor está conectado a la entrada PA1. La variación de la resistencia del LDR en función de la luz incidente provocará un cambio en el voltaje analógico en PA1, permitiendo al microcontrolador leer la intensidad lumínica ambiental.

Cada pulsador de los vistos en la iamgen 2.1 está configurado de manera que las entradas de señal (PC10, PD2, PC2, PC1) no reciben tensión ni corriente hasta que el botón sea presionado. Al presionar el botón, en el circuito se envía una señal al microcontrolador para que sea activada la luz LED correspondiente. Las cuatro LED están conectadas en serie con una resistencia limitadora de corriente de 100 Ω para proteger el componente. Los ánodos de los LEDs reciben la señal de control desde las entradas PA6, PB1, PB0 y PA7, mientras que los cátodos están conectados a tierra común.

## 3.3 Descripción del comportamiento

Podemos ver cómo se comporta el sistema a partir de la imagen 3.5, pasando por los estados de menú de acuerdo a lo que el usuario haga; para ejecutar los distintos eventos, se deben presionar los distintos pulsadores. Las instrucciones para cuál pulsador presionar se ven en el estado ST_MEN_MENU; luego de esa introducción se pasa al ST_MEN_MENU1, donde se verá la opción de seleccionar el modo de juego en ST_MEN_MENU3 o ver el historial de puntajes en ST_MEN_MENU3. 

  <div align="center">
  <img width="941" height="432" alt="image" src="https://github.com/user-attachments/assets/60a716ed-63ae-4e5f-8e8b-5b06931d566d" />

  <p><em>Imagen 3.5: Diagrama de estados.</em></p>
</div>

Pasando al ST_MEN_MENU2, allí se seleccionará el modo de juego, siendo difícil o normal; cada uno, al ser seleccionado, pasará a su respectivo evento donde se desarrollará el juego.

Finalmente, una vez que finalice el juego, sin importar el modo anteriormente seleccionado, se pasará al evento EV_GAME_OVER, que nos dejará en el estado ST_MEN_MENU4. Ahí se visualizará una indicación de fin de juego y se pasará a ST_MEN_MENU1 nuevamente.

## 3.4 Firmware del Simon Says:
Este proyecto implementa un enfoque orientado a eventos y polling no bloqueante, tal que en varios módulos hay implementadas funciones interfaz que permiten comunicación entre diferentes máquinas de estados, a continuación en la imagen 3.6 un ejemplo en el que se puede ver cómo ayuda esta metodología a respetar un aspecto básico como lo es la modularización en este proyecto:

  <div align="center">
  <img width="857" height="170" alt="image" src="https://github.com/user-attachments/assets/e48f5359-6c7d-4efc-a687-6680d2200db9" />
  <p><em>Imagen 3.6: Fragmento de código de task_gameplay.c.</em></p>
</div>

### 3.4.1 Task actuator

Módulo encargado de administrar estados básicos de los leds mediante una MEF. Esto permite desacoplar la lógica del juego del manejo directo de los pines. 

### 3.4.2 Task sensor

Este módulo es responsable de la gestión de la interfaz de entrada física mediante una MEF. Su función principal es realizar el filtrado digital de las señales (software debouncing) para eliminar los rebotes mecánicos inherentes a los botones.

### 3.4.3 Task adc

Esta tarea administra el Conversor Analógico-Digital (ADC) del STM32F103RB. Se encarga de disparar la conversión del canal conectado al sensor LDR (resistencia dependiente de la luz) y realizar un promedio de las lecturas para filtrar ruido eléctrico. El valor digital resultante representa la intensidad de luz ambiental y es puesto a disposición del sistema para el ajuste de brillo.

### 3.4.4 Task pwm

Responsable de la gestión de los Timers (TIM2 y TIM3) configurados en modo Pulse Width Modulation. Este módulo toma el valor procesado por la task adc y ajusta dinámicamente el Duty Cycle (ciclo de trabajo) de las señales que alimentan los LEDs. Su objetivo es mantener una visibilidad óptima de la secuencia de juego, aumentando la intensidad en ambientes iluminados y atenuándola en la oscuridad para confort visual.

### 3.4.5 Task gameplay
Es el núcleo lógico del proyecto. Implementa una  MEF que gestiona las reglas de "Simon Dice". Sus responsabilidades incluyen:
- Generación de la secuencia pseudoaleatoria incremental.
- Control de los tiempos de espera entre turnos.
- Validación de la entrada del usuario contra la secuencia guardada.
- Gestión de los niveles de dificultad ("Normal" y "Difícil").
- Determinación de las condiciones de victoria o derrota (Game Over).

### 3.4.6 Task storage

Este módulo implementa la lógica de persistencia de datos. Se encarga de verificar si el puntaje obtenido al finalizar una partida califica como un "Récord Histórico". Si es así, gestiona la estructura de datos (Puntaje + Iniciales) y solicita su escritura en la memoria no volátil. Al inicio del sistema, recupera y ordena estos datos para su visualización.

### 3.4.7 Task i2c

Módulo de bajo nivel que implementa el protocolo de comunicación I2C. Provee primitivas (Start, Stop, Write Byte, Read Ack) a la task storage. Su función crítica es manejar los tiempos de escritura (tWR) de la memoria EEPROM y asegurar la integridad de la transmisión de datos sin bloquear el resto del sistema.

### 3.4.8 Task display

Actúa como MEF intermedia entre display.c (librería de bajo nivel de abstracción) y task menu (manejo de display de alto nivel). Gestiona un buffer de pantalla para optimizar la escritura, verificando que se imprima 1 caracter por cada milisegundo.

### 3.4.9 Task menu

Módulo encargado de la navegación del sistema cuando no se está en una partida activa. Gestiona las pantallas de bienvenida, la selección de dificultad y la visualización de los puntajes históricos. Interpreta las pulsaciones de los botones (provenientes de task sensor) como comandos de navegación ("Izquierda", "Derecha", "Enter" y "Back") en lugar de colores de juego.


# 4. Ensayos y resultados
## 4.1 Medición y análisis de consumo

Para evaluar la eficiencia energética del sistema, se realizaron mediciones de corriente en dos escenarios operativos: **Modo Activo** (*Run Mode*) y **Modo Bajo Consumo** (*Sleep Mode*).

Las mediciones se dividen en dos categorías:
1.  **Consumo de periféricos:** Componentes externos (LEDs, Display, Sensores). Su consumo es independiente del estado del microcontrolador.
2.  **Consumo del microcontrolador (MCU):** Medido a través del *Jumper IDD* (JP5) de la placa Nucleo.

Para evaluar la eficiencia energética del sistema, se realizaron mediciones de corriente en la tabla 4.1 en dos escenarios operativos: **Modo Activo** (*Run Mode*) y **Modo Bajo Consumo** (*Sleep Mode*).

<div align="center">
  <table border="1" style="border-collapse: collapse; text-align: center;">
    <thead>
      <tr style="background-color: #f2f2f2;">
        <th>Componente / Medición</th>
        <th>Corriente <br>(SIN Sleep Mode)</th>
        <th>Corriente <br>(CON Sleep Mode)</th>
        <th>Observaciones</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td align="left"><strong>MCU (Jumper JP5 - IDD)</strong></td>
        <td><strong>38,8 mA</strong></td>
        <td><strong>38,8 mA</strong></td>
        <td align="left">Consumo del microcontrolador.</td>
      </tr>
      <tr>
        <td align="left">LED Azul (ON)</td>
        <td>0,146 mA</td>
        <td>0,146 mA</td>
        <td align="left">Consumo constante si está encendido.</td>
      </tr>
      <tr>
        <td align="left">LED Rojo (ON)</td>
        <td>0,120 mA</td>
        <td>0,120 mA</td>
        <td align="left">Consumo constante si está encendido.</td>
      </tr>
      <tr>
        <td align="left">Pulsador (Presionado)</td>
        <td>0,333 mA</td>
        <td>0,333 mA</td>
        <td align="left">Depende de resistencia interna.</td>
      </tr>
      <tr>
        <td align="left">Sensor LDR</td>
        <td>3 µA</td>
        <td>3 µA</td>
        <td align="left">Divisor de tensión resistivo.</td>
      </tr>
      <tr>
        <td align="left">Display LCD</td>
        <td>[-] mA</td>
        <td>[-] mA</td>
        <td align="left">-</td>
      </tr>
      <tr>
        <td align="left">Memoria EEPROM</td>
        <td>[-] mA</td>
        <td>[-] mA</td>
        <td align="left">-</td>
      </tr>
    </tbody>
  </table>
  <p><em>Tabla 4.1: Comparativa de consumo de corriente en los distintos modos de operación.</em></p>
</div>

<p><strong>Conclusión del análisis de consumo:</strong><br>

Tras realizar las pruebas comparativas, se observó que la corriente consumida por cada componente no registra variaciones perceptibles al activar el modo Sleep.

Nota: Dada esta ausencia de diferencia significativa en el consumo base del MCU, se optó por no extender las mediciones detalladas de los periféricos en dicho modo, asumiendo los valores del modo activo como referencia para el peor caso.</p>

## 4.2 Medición y análisis de tiempos de ejecución (WCET)

Se determinó el Worst Case Execution Time (WCET) de cada tarea del sistema utilizando el Timer DWT (Data Watchpoint and Trace) del microcontrolador para contar ciclos de reloj exactos antes y después de cada función task_update, convirtiendo luego esos ciclos a microsegundos. 
Este parámetro es crítico para garantizar que el sistema cumpla con los requisitos de tiempo real (Soft Real-Time) y no se produzca pérdida de eventos.

<p>A continuación en la tabla 4.2 se detallan los tiempos de ejecución medidos (en $\mu s$) y el porcentaje de uso de CPU para cada tarea, dependiendo del estado en el que se encuentra la Máquina de Estados del sistema.</p>

<div align="center">
  <table border="1" style="border-collapse: collapse; text-align: center;">
    <thead>
      <tr style="background-color: #f2f2f2;">
        <th>Tarea / Estado ($t$)</th>
        <th>t=0<br><sub></th>
        <th>t=1<br><sub></th>
        <th>t=2<br><sub></th>
        <th>t=3<br><sub></th>
        <th>t=4<br><sub></th>
        <th>t=5<br><sub></th>
        <th>t=6<br><sub></th>
        <th>t=7<br><sub></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td align="left"><strong>Task sensor</strong></td>
        <td>12</td>
        <td>13</td>
        <td>13</td>
        <td>13</td>
        <td>13</td>
        <td>13</td>
        <td>13</td>
        <td>13</td>
      </tr>
      <tr>
        <td align="left"><strong>Task actuator</strong></td>
        <td>4</td>
        <td>5</td>
        <td>5</td>
        <td>5</td>
        <td>5</td>
        <td>5</td>
        <td>5</td>
        <td>5</td>
      </tr>
      <tr>
        <td align="left"><strong>Task display</strong></td>
        <td>4</td>
        <td>76</td>
        <td>76</td>
        <td>76</td>
        <td>76</td>
        <td>76</td>
        <td>76</td>
        <td>76</td>
      </tr>
      <tr>
        <td align="left"><strong>Task menu</strong></td>
        <td>3</td>
        <td>72</td>
        <td>78</td>
        <td>88</td>
        <td>128</td>
        <td>128</td>
        <td>128</td>
        <td>128</td>
      </tr>
      <tr>
        <td align="left"><strong>Task ADC</strong></td>
        <td>20</td>
        <td>20</td>
        <td>20</td>
        <td>20</td>
        <td>20</td>
        <td>20</td>
        <td>20</td>
        <td>20</td>
      </tr>
      <tr>
        <td align="left"><strong>Task PWM</strong></td>
        <td>2</td>
        <td>3</td>
        <td>3</td>
        <td>3</td>
        <td>3</td>
        <td>3</td>
        <td>3</td>
        <td>3</td>
      </tr>
      <tr>
        <td align="left"><strong>Task gameplay</strong></td>
        <td>4</td>
        <td>9</td>
        <td>9</td>
        <td>9</td>
        <td>9</td>
        <td>9</td>
        <td>9</td>
        <td>9</td>
      </tr>
      <tr>
        <td align="left"><strong>Task storage</strong></td>
        <td>199</td>
        <td>199</td>
        <td>199</td>
        <td>199</td>
        <td>578</td>
        <td>578</td>
        <td>578</td>
        <td>578</td>
      </tr>
      <tr style="border-top: 2px solid #000; background-color: #fffde7;">
        <td align="left"><strong>CPU Usage (%)</strong></td>
        <td>24,8%</td>
        <td>39,7%</td>
        <td>40,3%</td>
        <td>41,3%</td>
        <td>83,2%</td>
        <td>83,2%</td>
        <td>83,2%</td>
        <td>82,9%</td>
      </tr>
    </tbody>
  </table>
  <p><em>Tabla 4.2: Evolución del WCET y carga del CPU según el estado del juego.</em></p>
</div>

<p><strong>Observaciones:</strong><br>
Se evidencia claramente cómo la tarea de almacenamiento (<em>Task Storage</em>) impacta en el rendimiento general. En los estados donde se requiere gestión de puntajes (t=4 a t=7), el tiempo de ejecución de esta tarea salta a 578 $\mu s$</strong>, elevando el uso del CPU por encima del 80%. Sin embargo, el sistema se mantiene estable dentro del límite de tiempo real.</p>

## 4.3 Cálculo del Factor de Uso (U) de la CPU

El factor de utilización del procesador determina la viabilidad del sistema (planificabilidad). Para un sistema cíclico ejecutivo, la condición necesaria es que la suma de los tiempos de ejecución no supere el periodo del sistema ($U < 100\%$).

La fórmula general aplicada es:

$$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \times 100$$

Donde:
* $C_i$: es el WCET (*Worst Case Execution Time*) de la tarea $i$.
* $T_i$: es el periodo del planificador (**1000 $\mu s$**).

**Verificación en el Peor Escenario (Worst Case Scenario):**
Tomando los valores críticos medidos en la sección 4.2 (cuando la escritura en EEPROM y el menú están activos simultáneamente), se obtiene la siguiente sumatoria:

$$U_{max} = \frac{13 + 5 + 76 + 128 + 20 + 3 + 9 + 578}{1000} \times 100$$

$$U_{max} = \frac{832 \mu s}{1000 \mu s} \times 100$$

$$U_{max} = \mathbf{83,2\%}$$

**Conclusión:**
El factor de uso máximo calculado es del **83,2%**. Dado que $U < 100\%$, se concluye que el sistema es **planificable**. El procesador dispone de un margen libre (*Slack Time*) del **16.8%** (~168 $\mu s$ por ciclo), tiempo durante el cual entra en modo *Sleep* para reducir el consumo energético.

## 4.4 Cumplimiento de requisitos  

Una vez finalizado el trabajo, se realizó la tabla 4.3 con los requisitos iniciales, agregando el estado de los mismos.

| Grupo | ID | Descripción | Estado |
| :---- | :---- | :---- | :---- |
| Juego | 1.1 | El sistema generará una secuencia de LEDs pseudoaleatoria de longitud creciente. | Completo 🟢 |
|  | 1.2 | El sistema permitirá al jugador repetir la secuencia mediante cuatro pulsadores asociados a los cuatro LEDs. | Completo 🟢 |
|  | 1.3 | El sistema comparará la secuencia ingresada por el jugador con la secuencia objetivo y determinará si es correcta. | Completo 🟢 |
|  | 1.4 | En caso de acierto, el sistema incrementará la longitud de la secuencia y avanzará al siguiente nivel. | Completo 🟢 |
|  | 1.5 | En caso de error, el sistema finalizará la ronda y mostrará el resultado al jugador. | Completo 🟢 |
|  | 1.6 | En modo Normal, al iniciar cada nivel se reproducirá la secuencia completa acumulada. | Completo 🟢 |
|  | 1.7 | En modo Difícil, al iniciar cada nivel solo se reproducirá el **nuevo color agregado** a la secuencia. | Completo 🟢 |
| Interfaz luminosa | 2.1 | Cada LED estará asociado a un color fijo y a un pulsador específico. | Completo 🟢 |
|  | 2.2 | Durante la reproducción de la secuencia, el LED correspondiente se encenderá de forma claramente distinguible. | Completo 🟢 |
|  | 2.3 | Al presionar un pulsador, el LED asociado se encenderá mientras dure la pulsación. | Completo 🟢 |
|  | 2.4 | El sistema implementará antirrebote por software para los cuatro pulsadores. | Completo 🟢 |
|  | 2.5 | El sistema deberá registrar pulsaciones rápidas sin perder eventos. | Completo 🟢 |
|  | 2.6 | El brillo de los LEDs se ajustará automáticamente según el valor leído en el sensor LDR. | Completo 🟢 |
| Sensor LDR | 3.1 | El sistema contará con un sensor de luz LDR conectado a una entrada analógica del STM32. | Completo 🟢 |
|  | 3.2 | El sistema leerá al iniciar el valor de la LDR mediante el ADC. | Completo 🟢 |
|  | 3.3 | El sistema ajustará el ciclo de trabajo PWM de los LEDs en función de la luminosidad ambiente. | Completo 🟢 |
| Pantalla LCD | 4.1 | El sistema contará con una pantalla LCD para mostrar información de estado. | Completo 🟢 |
|  | 4.2 | Al encender el sistema, el LCD mostrará una pantalla de bienvenida. | Completo 🟢 |
|  | 4.3 | Luego de la bienvenida, el LCD mostrará una pantalla de selección de dificultad (Normal / Difícil). | Completo 🟢 |
|  | 4.4 | Durante el juego, el LCD mostrará el puntaje actual del jugador. | Completo 🟢 |
|  | 4.5 | Al apagar o finalizar el juego, el LCD mostrará una pantalla de despedida. | Completo 🟢 |
| Menú con botones | 5.1 | Todos los menús se manejarán exclusivamente con los cuatro botones del juego. | Completo 🟢 |
|  | 5.2 | Al menos un botón permitirá avanzar entre opciones y otro confirmará la selección. | Completo 🟢 |
|  | 5.3 | El sistema indicará en pantalla las opciones seleccionadas y confirmadas. | Completo 🟢 |
| Audio | 6.1 | El sistema contará con un buzzer para producir señales sonoras. | Cancelado 🔴 |
|  | 6.2 | Cada color del juego tendrá asignado un tono característico reproducido por el buzzer. | Cancelado 🔴 |
|  | 6.3 | El sistema reproducirá una melodía corta al superar un nivel. | Cancelado 🔴 |
|  | 6.4 | El sistema reproducirá una secuencia de error cuando el jugador falle la secuencia. | Cancelado 🔴 |
|  | 6.5 | El sistema podrá emitir sonidos breves al navegar por el menú. | Cancelado 🔴 |
| Modos de juego y dificultad | 7.1 | El sistema contará al menos con dos niveles de dificultad: Normal y Difícil. | Completo 🟢 |
|  | 7.2 | La dificultad podrá afectar la velocidad de reproducción de la secuencia y/o el tiempo de respuesta permitido. | Completo 🟢 |
|  | 7.3 | En Normal se reproducirá la secuencia completa en cada nivel; en Difícil, solo el nuevo color agregado. | Completo 🟢 |
| Persistencia y estadísticas (EEPROM) | 8.1 | El sistema almacenará el puntaje máximo alcanzado en memoria EEPROM externa. | Completo 🟢 |
|  | 8.2 | El sistema permitirá leer y mostrar el puntaje máximo guardado al inicio o desde un menú de estadísticas. | Completo 🟢 |
|  | 8.3 | El sistema permitirá reiniciar el récord (borrar el puntaje máximo guardado) desde el menú. | Cancelado 🔴 |
|  | 8.4 | La EEPROM podrá usarse para almacenar configuraciones de dificultad u otros parámetros del juego. | Completo 🟢 |
| Seguridad y robustez | 9.1 | El sistema deberá iniciar siempre en un estado seguro, con LEDs y buzzer apagados hasta que el usuario interactúe. | Completo 🟢 |
|  | 9.2 | El sistema organizará su lógica en una máquina de estados para evitar bloqueos y comportamientos impredecibles. | Completo 🟢 |
|  | 9.3 | El sistema deberá indicar mediante mensajes en la pantalla y señales sonoras si ocurre un error interno o condición inesperada. | Cancelado 🔴 |

<p align="center"><em>Tabla 4.3: Cumplimiento de requisitos</em></p>

Se observa que la gran mayoría de los requisitos se cumplieron para este proyecto. Solamente resta agregar el audio que no se realizó debido al tiempo límite del proyecto. Este siendo el requisito más inconsecuente de todos; por más que sea una ayuda al jugador una indicación de audio, esto no le impide el juego al usuario y puede ser usado de todas maneras.


## 4.5 Reporte de uso
Se muestra el "Console" en la imagen 4.1 como también el "Build Analyzer" en la figura 4.2:
  <div align="center">
  <img width="550" height="156" alt="Captura de pantalla 2026-02-17 091647" src="https://github.com/user-attachments/assets/4dcc685b-f506-4dbb-94a7-4577b9bfcd3b" />
  <p><em>Imagen 4.1: Resumen del proceso de compilación (Build Output).</em></p>
</div>
  <div align="center">
  <img width="696" height="62" alt="Captura de pantalla 2026-02-17 093125" src="https://github.com/user-attachments/assets/fc268403-327b-462b-ab67-52ebe046923a" />
  <p><em>Imagen 4.2: Reporte de ocupación de memoria (Size Report).</em></p>
</div>

Se observa que el firmware actual ocupa el 26,81% de la capacidad de almacenamiento y el 14,84% de la memoria dinámica disponible. Esto indica que el sistema posee un amplio margen de recursos excedentes , permitiendo futuras expansiones o la adición de funcionalidades complejas sin riesgo de desbordamiento de memoria (Stack Overflow) o falta de espacio en el disco.

## 4.6 Prueba de integración 

Se dejará adjunto un link funcional al video: https://youtu.be/UB5wlkK1kCw

# 5.Conclusiones

## 5.1 Resultados obtenidos

Para concluir el informe y el trabajo, pudimos integrar los componentes con las aplicaciones y programas requeridos, logrando su ejecución sin un delay excesivo y con máxima eficiencia. Mientras que hubo dificultades, como por ejemplo la integración del modo sleep, por su mayor parte estas pudieron ser resueltas de manera exitosa. 

Tomamos como aprendizaje los funcionamientos de los sistemas embebidos y cómo estos son integrados a variedades de proyectos con mucho alcance y uso en el ámbito profesional.

## 5.2 Próximos pasos

Para continuar este proyecto, podríamos rediseñar la interfaz del usuario para que, con un mayor presupuesto, podamos agregar una carcasa exterior y botones más grandes que sean más satisfactorios para presionar y así brindar una mejor experiencia al usuario. También consideramos agregar el sonido que no pudimos incluir por restricciones de tiempo.

# 6.Bibliografía
[1]https://www.alldatasheet.com/datasheet-pdf/view/75272/MICRO-ELECTRONICS/MBB51D.html 

[2]https://www.alldatasheet.com/datasheet-pdf/view/574755/ATMEL/AT24C256.html 

[3]https://www.nubbeo.com.ar/productos/modulo-memoria-at24c256-i2c-256kbits-32kbytes-arduino-nubbeo/

[4]https://embeddedprojects101.com/how-to-interface-an-i2c-eeprom-with-stm32/#:~:text=I2C%20de%20Microchip.-,Configuraci%C3%B3n%20de%20pines%20y%20direcciones%20de%20la%20EEPROM,En%20nuestro%20caso%2C%20son%201010.

[5]https://deepbluembedded.com/stm32-eeprom-flash-emulation-fee-read-write-examples/#:~:text=STM32%20microcontrollers%20don't%20have,to%20implement%20in%20this%20tutorial. 

[6]https://wiki.st.com/stm32mcu/wiki/Getting_started_with_I2C 

[7]https://campusgrado.fi.uba.ar/course/view.php?id=1217&section=21#tabs-tree-starthttps://campusgrado.fi.uba.ar/course/view.php?id=1217&section=22#tabs-tree-start

[8]https://web.alfredstate.edu/faculty/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html

[9]https://www.arm.com/resources/education/books/designing-embedded-systems

[10]https://www.sciencedirect.com/science/article/pii/S2405844020309919

[11]https://www.sciencedirect.com/science/article/abs/pii/S0003687012001226
