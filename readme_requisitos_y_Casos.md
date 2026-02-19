

 <img src="https://github.com/user-attachments/assets/15600b18-f73b-4ba3-a959-47f0048a1ab6" alt="image2" width="50%">


# **Juego Interactivo "Simón dice"**

 

**Autores: Tomás Ignacio Condo, Camila Trinidad Morasso y Francisco Javier Briones**

**Padrones: 111.457, 110.790, 110.058**

**Fecha: 2do cuatrimestre 2025**

 

 

 ### **1. Selección del proyecto a implementar**

#### **1.1 Objetivo del proyecto y resultados esperados**

El objetivo de este proyecto es diseñar e implementar un juego interactivo del tipo *“Simón dice”* . El sistema generará secuencias luminosas crecientes de manera pseudoaleatoria, empleando cuatro LEDs de distintos colores asociados a cuatro pulsadores que el usuario deberá utilizar para reproducir la secuencia mostrada, tal que cada secuencia sea mayor que la anterior.

Como objetivos adicionales, el sistema incluirá:

- Un **mecanismo de ajuste automático de brillo** mediante un sensor analógico de luz (LDR), que permitirá adaptar la intensidad de los LEDs según el nivel de iluminación del ambiente.
- Un **menú completo controlado exclusivamente con los mismos cuatro botones del juego**, siendo que el sistema estará subordinado a un estado diferente al del juego en el momento del manejo del menú donde cada botón corresponderá a atrás, aceptar, arriba o abajo, disponiendo de diferentes pantallas:  
  1. Pantalla de bienvenida al encender el sistema.  
  2. Pantalla de selección de dificultad (modo *Normal* y modo *Difícil*).
  3. Pantalla de visualización de top 3 puntajes históricos.
  4. Pantalla de juego, donde se muestra el puntaje durante la partida.
  5. Pantalla de despedida al apagar el juego.
- Un **modo de dificultad especial “Difícil”**, en el que al agregar un nuevo color a la secuencia solo se reproduce ese **nuevo** color, y no la secuencia completa (a diferencia del modo Normal, donde siempre se reproduce toda la secuencia acumulada).
- Una **memoria EEPROM externa**, destinada a almacenar datos persistentes como puntajes máximos, configuraciones de dificultad y, eventualmente, secuencias u otras estadísticas del juego.

Se pretende que el sistema sea robusto frente a rebotes mecánicos de los pulsadores, errores de temporización y entradas inválidas del usuario, utilizando una arquitectura modular basada en máquinas de estados. Al finalizar el proyecto se espera obtener un prototipo funcional, configurable, expandible y con capacidad de almacenamiento persistente para mejorar la experiencia de juego y permitir extensiones futuras.  



#### **1.2 Proyectos similares**

Se consideran tres tipos de proyectos posibles que cumplen con los objetivos y resultados esperados del trabajo, todos basados en juegos electrónicos de memoria con secuencias luminosas:

1. **Juego “Simón dice” Avanzado con STM32**, con cuatro LEDs, cuatro pulsadores, selección en menú interactivo, registro de puntaje y adaptabilidad al ambiente.
2. **Juego “Simón dice” Básico con STM32**, sólo con leds y teclas, sin interfaz interativa y de un tamaño reducido.  
3. **Juego “Simón dice” Básico con Arduino UNO**, similar a la versión básica.

Para comparar estas alternativas, se tienen en cuenta seis aspectos característicos:

1. **Disponibilidad del hardware:** Se evalúa si el proyecto es fácilmente implementable con hardware disponible en mercados regionales.  
2. **Calidad de la experiencia de juego:** Se refiere a qué tan interesante, inmersivo y variado resulta el juego para el usuario final (feedback visual, menús, niveles de dificultad, etc.).  
3. **Escalabilidad del sistema:** Se entiende como la facilidad para extender el proyecto en el futuro, incorporando nuevos modos de juego, más niveles de dificultad, almacenamiento de estadísticas adicionales, comunicación con PC, etc.  
4. **Tiempo de implementación:** Tiempo total que llevará diseñar, programar, integrar y depurar el proyecto seleccionado dentro del cuatrimestre.  
5. **Costo:** El costo total del proyecto, incluyendo la placa de desarrollo, la pantalla LCD, el buzzer, el sensor LDR, la EEPROM, los pulsadores, LEDs, resistencias y cableado.  
6. **Interés personal:** El grado de motivación e interés en trabajar con la plataforma STM32, aprender a utilizar sus periféricos (GPIO, ADC, timers, I2C/SPI, etc.) y desarrollar un juego que resulte atractivo de usar y de mejorar.  

Se decide ponderar los aspectos de la siguiente manera, del 1 al 10:

- Disponibilidad del hardware: 10  
- Calidad de la experiencia de juego: 8  
- Escalabilidad del sistema: 6  
- Tiempo de implementación: 8  
- Costo: 5  
- Interés personal: 8  

La disponibilidad del hardware es crítica, ya que se busca trabajar con componentes accesibles y fáciles de conseguir sin depender de importaciones costosas o de plazos largos; por ello se asigna un peso 10. La calidad de la experiencia de juego se valora con peso 8, dado que el objetivo principal es obtener un juego entretenido y rejugable, con buena realimentación visual y sonora. La escalabilidad del sistema recibe un peso 6: es deseable que el proyecto sea ampliable, pero se prioriza primero obtener un prototipo estable y completo. El tiempo de implementación se pondera con 8, ya que se dispone de un período acotado y se requieren iteraciones de prueba y corrección de errores.

El costo tiene un peso 5, porque, si bien no es irrelevante, se prioriza el aprendizaje y la calidad del proyecto por sobre una minimización extrema del presupuesto. Por último, el interés personal se valora con 8, ya que se busca realizar un proyecto motivador, que permita practicar programación embebida sobre STM32 y que resulte atractivo para seguir ampliándolo más allá de la materia.  

La siguiente tabla (Tabla 1.2.1) muestra los valores ponderados asignados a cada proyecto considerado.  


<table>
    <thead>
        <tr>
            <th rowspan="2">Proyecto</th>
            <th colspan="2">Simón avanzado STM32<br>(LCD, LDR, EEPROM)</th>
            <th colspan="2">Simón básico STM32<br>(solo LEDs y pulsadores)</th>
            <th colspan="2">Simón básico Arduino UNO</th>
        </tr>
        <tr>
            <th>Puntaje</th>
            <th>Puntaje Ponderado</th>
            <th>Puntaje</th>
            <th>Puntaje Ponderado</th>
            <th>Puntaje</th>
            <th>Puntaje Ponderado</th>
        </tr>
    </thead>
    <tbody>
        <tr class="header-row">
            <td align ="center">Disponibilidad de Hardware <br>(peso : 10)</td>
            <td>8</td>
            <td>80</td>
            <td>9</td>
            <td>90</td>
            <td>10</td>
            <td>100</td>
        </tr>
        <tr>
            <td align ="center">Calidad de la experiencia de juego <br>(peso : 8)</td>
            <td>10</td>
            <td>80</td>
            <td>7</td>
            <td>56</td>
            <td>5</td>
            <td>40</td>
        </tr>
        <tr class="header-row">
            <td align ="center">Escalabilidad del sistema <br>(peso : 6)</td>
            <td>10</td>
            <td>60</td>
            <td>7</td>
            <td>42</td>
            <td>5</td>
            <td>30</td>
        </tr>
        <tr>
            <td align ="center">Tiempo de Implementación <br>(peso : 8)</td>
            <td>6</td>
            <td>48</td>
            <td>8</td>
            <td>64</td>
            <td>9</td>
            <td>72</td>
        </tr>
        <tr class="header-row">
            <td align ="center">Costo (peso : 5)</td>
            <td>6</td>
            <td>30</td>
            <td>8</td>
            <td>40</td>
            <td>9</td>
            <td>45</td>
        </tr>
        <tr>
            <td align ="center">Interés personal <br>(peso : 8)</td>
            <td>10</td>
            <td>80</td>
            <td>8</td>
            <td>64</td>
            <td>6</td>
            <td>48</td>
        </tr>
        <tr class="highlight-green">
            <td>Puntaje Total</td>
            <td>-</td>
            <td>378</td>
            <td>-</td>
            <td class="highlight-red">356</td>
            <td>-</td>
            <td>335</td>
        </tr>
    </tbody>
</table>
<p align="center"><em>Tabla 1.2.1: Comparación de proyectos</em></p>



#### **1.3 Selección de proyecto**

Considerando la Tabla 1.2.1, se elige implementar el juego “Simón dice” avanzado con STM32, utilizando LEDs, pulsadores, pantalla LCD, buzzer, sensor LDR y memoria EEPROM externa. Si bien el juego básico con Arduino UNO presenta una excelente disponibilidad de hardware y un costo reducido, no permite aprovechar en profundidad los periféricos del STM32 ni desarrollar una arquitectura modular más compleja. Además, limita la posibilidad de escalar el proyecto hacia nuevas funcionalidades, como distintos modos de juego, almacenamiento de récords o futuras comunicaciones con otros dispositivos.

Por otro lado, el juego “Simón dice” básico con STM32 constituye una opción intermedia: mantiene la plataforma STM32 pero con una interfaz simple, sin menú, sin ajuste automático de brillo y sin persistencia de datos. Sus tiempos de implementación son menores, pero la experiencia de juego es más limitada y se desaprovecha la oportunidad de trabajar con periféricos adicionales.

El juego avanzado con STM32 se destaca porque combina:

- Una **experiencia de usuario más rica** (secuencias luminosas, sonidos asociados a cada color, melodías de acierto y error).  
- Un **menú de múltiples pantallas** (bienvenida, selección de dificultad, puntaje en juego y despedida), controlado únicamente con los mismos cuatros botones del juego.  
- La **adaptación automática del brillo** mediante el sensor LDR.  
- La **persistencia de datos** mediante EEPROM externa.  

Esto lo convierte en un proyecto ideal para integrar conocimientos de programación embebida, diseño de máquinas de estados, gestión de tiempos, uso de ADC y buses de comunicación, y modularidad del código.

En el contexto local, existen numerosos juegos comerciales del tipo “Genius” o “Simón” disponibles como juguetes o aplicaciones de celular. Sin embargo, suelen ser dispositivos cerrados que no permiten modificar las reglas del juego ni acceder a su implementación interna. Este proyecto apunta a desarrollar una plataforma abierta y programable, pensada tanto para entretenimiento como para aprendizaje en el ámbito de la ingeniería electrónica, donde el alumno pueda experimentar con nuevas reglas, efectos sonoros, modos de dificultad y sistemas de almacenamiento de récords.

Los desafíos principales del proyecto se centran en la correcta generación de secuencias pseudoaleatorias, la sincronización precisa de tiempos para la reproducción de la secuencia y la lectura de las respuestas del usuario, el diseño de un esquema robusto de antirrebote, la implementación de un menú navegable solo con los botones de juego y la organización del firmware en módulos y máquinas de estado claras, que faciliten la depuración y la futura expansión del juego.  



###### **1.3.1 Diagrama en bloques**

En la Figura 1.3.1 se muestra el diagrama en bloques del sistema con los principales módulos del proyecto.  

<p align="center">
  <img src="Diagrama_en_Bloques.png" alt="Diagrama en bloques del sistema SimonDice STM32" width="450">
</p>

<p align="center"><em>Figura 1.3.1: Diagrama en bloques del sistema</em></p>



---

### **2. Elicitación de requisitos y casos de uso**

En el mercado argentino existen diversos productos relacionados con juegos de memoria y reflejos, desde el clásico *“Simon”* de Hasbro hasta juguetes electrónicos genéricos y aplicaciones móviles que imitan el mismo concepto. Estos dispositivos suelen ofrecer distintas combinaciones de luces y sonidos, pero en general son sistemas cerrados, con lógica de juego fija y sin posibilidad de modificación por parte del usuario.

Como primer competidor puede mencionarse el juego electrónico comercial tipo *“Simon”*, que ofrece una secuencia de luces y sonidos que el jugador debe repetir. Si bien la experiencia es entretenida, el usuario no tiene acceso al hardware ni al software interno, por lo que no puede cambiar reglas, niveles de dificultad ni integrar el juego con otros sistemas. Nuestro proyecto se diferencia en que está completamente implementado sobre una plataforma de desarrollo STM32, permitiendo modificar el firmware y experimentar con nuevas funciones.

Un segundo competidor lo constituyen las aplicaciones de juego de memoria para teléfonos celulares, ampliamente disponibles en tiendas digitales. Estas aplicaciones pueden ofrecer gráficos y sonidos avanzados, pero pierden el componente tangible de interactuar con botones físicos y LEDs reales, y no permiten practicar el diseño de sistemas embebidos. El presente proyecto recupera ese aspecto físico y educativo, brindando un entorno ideal para ejercitar programación en C sobre STM32 y diseño de interfaces hombre-máquina simples pero efectivas.

En resumen, el mercado de juegos electrónicos de memoria es amplio y competitivo, pero la posibilidad de personalización y experimentación técnica que ofrece un prototipo basado en STM32 permite desarrollar un producto único, orientado tanto al entretenimiento como a la formación académica, destacándose además por el ajuste dinámico de brillo (LDR), el menú manejado solo con botones y la presencia de memoria EEPROM externa.  



#### **2.1 Requisitos del proyecto**

En la Tabla 2.1 se detallan los principales requisitos funcionales del sistema.

| Grupo | ID | Descripción |
| :---- | :---- | :---- |
| Juego | 1.1 | El sistema generará una secuencia de LEDs pseudoaleatoria de longitud creciente. |
|  | 1.2 | El sistema permitirá al jugador repetir la secuencia mediante cuatro pulsadores asociados a los cuatro LEDs. |
|  | 1.3 | El sistema comparará la secuencia ingresada por el jugador con la secuencia objetivo y determinará si es correcta. |
|  | 1.4 | En caso de acierto, el sistema incrementará la longitud de la secuencia y avanzará al siguiente nivel. |
|  | 1.5 | En caso de error, el sistema finalizará la ronda y mostrará el resultado al jugador. |
|  | 1.6 | En modo Normal, al iniciar cada nivel se reproducirá la secuencia completa acumulada. |
|  | 1.7 | En modo Difícil, al iniciar cada nivel solo se reproducirá el **nuevo color agregado** a la secuencia. |
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

<p align="center"><em>Tabla 2.1: Requisitos del proyecto</em></p>

En las Tablas 2.2 a 2.4 se presentan 3 casos de uso para el sistema.  



#### **2.2 Caso de uso 1: El usuario juega una partida en modo clásico**

| Elemento | Definición |
| :---- | :---- |
| Disparador | El jugador quiere iniciar una nueva partida en modo clásico (Normal o Difícil). |
| Precondiciones | El sistema está encendido. Se ha mostrado la pantalla de bienvenida. El jugador ha seleccionado una dificultad en la pantalla correspondiente utilizando los botones del juego. Todos los LEDs están apagados y el buzzer en silencio. |
| Flujo principal | El jugador navega el menú utilizando los botones y selecciona la opción “Nuevo juego”. El sistema genera una secuencia pseudoaleatoria inicial de un solo LED y la reproduce con luz y sonido (según el modo: secuencia completa o solo el nuevo color). El jugador repite la secuencia utilizando los cuatro pulsadores; por cada pulsación correcta se enciende el LED correspondiente y se reproduce su tono. Si el jugador ingresa correctamente toda la secuencia, el sistema incrementa la longitud en un elemento, actualiza el nivel y el puntaje y muestra el nuevo puntaje en la pantalla LCD. Este ciclo se repite hasta que el jugador comete un error o decide abandonar la partida. |
| Flujos alternativos | a) El jugador se equivoca en alguna pulsación: el sistema reproduce un sonido de error, muestra un mensaje de “Secuencia incorrecta” en el LCD y finaliza la partida mostrando el puntaje final.  b) El jugador excede el tiempo máximo para ingresar la secuencia: el sistema considera la jugada como incorrecta, reproduce un sonido de error y finaliza la partida.  c) El jugador presiona una combinación de botones para cancelar la partida: el sistema detiene el juego, guarda el puntaje si corresponde (por ejemplo, si es un nuevo récord) y vuelve al menú principal. |

<p align="center"><em>Tabla 2.2: Caso de uso 1: El usuario juega una partida</em></p>



#### **2.3 Caso de uso 2: El usuario cambia la dificultad del juego**

| Elemento | Definición |
| :---- | :---- |
| Disparador | El jugador quiere cambiar el nivel de dificultad del juego (Normal / Difícil). |
| Precondiciones | El sistema está encendido. No hay una partida en curso. El juego se encuentra en el menú principal o en el menú de configuración. |
| Flujo principal | El jugador accede a la pantalla de selección de dificultad utilizando los botones del juego. En el LCD se muestran las opciones “Normal” y “Difícil”. Mediante uno o más botones se avanza entre las opciones y se selecciona la deseada con otro botón (por ejemplo, “Confirmar”). El sistema almacena la nueva dificultad seleccionada (posiblemente en EEPROM), actualiza los parámetros internos (forma de reproducir la secuencia, tiempos, etc.) y vuelve al menú principal mostrando la dificultad activa. |
| Flujos alternativos | a) El jugador sale del menú de dificultad sin confirmar ningún cambio (por ejemplo, con una combinación de botones): el sistema mantiene la dificultad previamente establecida.  b) Se produce una lectura inválida de los botones (por rebote o secuencia inconsistente): el sistema descarta la entrada y solicita al usuario que vuelva a seleccionar, manteniendo el estado anterior. |

<p align="center"><em>Tabla 2.3: Caso de uso 2: El usuario cambia la dificultad del juego</em></p>



#### **2.4 Caso de uso 3: El usuario consulta o reinicia el puntaje máximo**

| Elemento | Definición |
| :---- | :---- |
| Disparador | El jugador quiere consultar o reiniciar el puntaje máximo almacenado en la memoria EEPROM. |
| Precondiciones | El sistema está encendido. No hay una partida en curso. El juego se encuentra en el menú principal. La EEPROM ha sido inicializada correctamente. |
| Flujo principal | El jugador navega hasta el menú de “Estadísticas” utilizando los mismos botones del juego. El sistema lee de la EEPROM el puntaje máximo almacenado y lo muestra en la pantalla LCD, junto con el último puntaje obtenido (si se desea). El jugador puede desplazarse con los botones hasta la opción “Reiniciar récord” y seleccionarla con el botón de confirmación. El sistema solicita confirmación adicional (por ejemplo, mostrando “¿Confirmar borrado? Sí/No”). Si el jugador confirma, el sistema borra o reinicia el valor de récord en la EEPROM (por ejemplo, a 0) y muestra un mensaje indicando que el récord fue reiniciado correctamente. |
| Flujos alternativos | a) El jugador decide no reiniciar el récord al momento de la confirmación (por ejemplo, seleccionando “No” con los botones): el sistema conserva el puntaje máximo y vuelve al menú anterior.  b) Se produce un error en el acceso a la EEPROM (por ejemplo, fallo de comunicación): el sistema muestra un mensaje de error en el LCD, reproduce un sonido de falla, descarta la operación de borrado y deshabilita temporalmente la opción de reinicio hasta que se reinicie el dispositivo o se recupere la condición. |

<p align="center"><em>Tabla 2.4: Caso de uso 3: El usuario consulta o reinicia el puntaje máximo</em></p>

