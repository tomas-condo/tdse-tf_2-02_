/********************** inclusions *******************************************/
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "board.h"
#include "task_gameplay.h"
#include "task_gameplay_attribute.h"
#include "task_gameplay_interface.h"
#include "task_menu_interface.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"
#include "task_storage_attribute.h"
#include "task_storage_interface.h"
#include "i2c.h"
#include "task_adc.h"
#include "app.h"
#include "display.h"
#include "logger.h"

/********************** macros and definitions *******************************/
#define G_TASK_GAME_CNT_INI         0ul
#define G_TASK_GAME_TICK_CNT_INI    0ul

#define MAX_SEQUENCE_LEN    100

#define DEL_GAME_XX_MIN
#define DELAY_GAME_START       1000
#define DELAY_LED_ON_NORMAL    800
#define DELAY_LED_OFF_NORMAL   400
#define DELAY_LED_ON_HARD      400
#define DELAY_LED_OFF_HARD     200
#define DELAY_BETWEEN_ROUNDS   1000
#define DELAY_GAME_OVER        3000

#define GAME_DIFF_NORMAL    0
#define GAME_DIFF_HARD      1

#define OFF      0
#define ROJO 	 1
#define VERDE 	 2
#define AZUL     3
#define AMARILLO 4

// Array: Indice 0=OFF, 1=ROJO, 2=VERDE, 3=AZUL, 4=AMARILLO
const task_actuator_ev_t LED_MAP[] = {
    0,           // OFF
    ID_LED_RO,   // 1
    ID_LED_VE,   // 2
    ID_LED_AZ,   // 3
    ID_LED_AM    // 4
};

/********************** internal data declaration ****************************/
task_gameplay_dta_t task_gameplay_dta =
{
    0,
	ST_GAME_IDLE,
    EV_GAME_IDLE,
    false,
    false,

    {0},
    0,
    0,
    0,
    0,
    0,
    OFF,
	false,
};

#define GAMEPLAY_DTA_QTY	(sizeof(task_gameplay_dta)/sizeof(task_gameplay_dta_t))

uint16_t high_score1 = 0;
uint16_t high_score2 = 0;
uint16_t high_score3 = 0;
uint32_t brillo_juego = 0; // guardamos la luz ambiente
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_gameplay 		= "Task Gameplay (Interactive Gameplay)";
const char *p_task_gameplay_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_gameplay_cnt;
volatile uint32_t g_task_gameplay_tick_cnt;
extern ADC_HandleTypeDef hadc1;
/********************** external functions definition ************************/

void task_gameplay_init(void *parameters)
{
	task_gameplay_dta_t *p_task_gameplay_dta;
	task_gameplay_state_t	state;
	task_gameplay_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	/*LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_gameplay_init), p_task_gameplay);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_gameplay), p_task_gameplay_);*/

	/* Init & Print out: Task execution counter */
	g_task_gameplay_cnt = G_TASK_GAME_CNT_INI;
	//LOGGER_INFO("   %s = %lu", GET_NAME(g_task_gameplay_cnt), g_task_gameplay_cnt);

	init_queue_event_task_gameplay();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_gameplay_dta = &task_gameplay_dta;

	/* Init & Print out: Task execution FSM */
	state = ST_GAME_IDLE;/**/
	p_task_gameplay_dta->state = state;

	event = EV_GAME_IDLE;
	p_task_gameplay_dta->event = event;

	b_event = false;
	p_task_gameplay_dta->flag = b_event;

	/*LOGGER_INFO(" ");
	LOGGER_INFO("   %s = %lu   %s = %lu   %s = %s",
				 GET_NAME(state), (uint32_t)state,
				 GET_NAME(event), (uint32_t)event,
				 GET_NAME(b_event), (b_event ? "true" : "false"));*/

	p_task_gameplay_dta->score = 0;
    srand(HAL_GetTick()); // Inicializar semilla aleatoria

	put_event_task_actuator(EV_LED_XX_OFF, ID_LED_AZ);
	put_event_task_actuator(EV_LED_XX_OFF, ID_LED_AM);
	put_event_task_actuator(EV_LED_XX_OFF, ID_LED_RO);
	put_event_task_actuator(EV_LED_XX_OFF, ID_LED_VE);

	// --- LDR CODIGO ---
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 20) == HAL_OK)
	{
	    // A. Lectura cruda (Rango 0 a 4095)
	    uint32_t raw_adc = HAL_ADC_GetValue(&hadc1);

	    brillo_juego = raw_adc << 4;

	    if(brillo_juego < 8000)
	        brillo_juego = 8000;

	    if (brillo_juego > 65535)
	        brillo_juego = 65535;
	}
	HAL_ADC_Stop(&hadc1);


	high_score1 = eeprom_read_score(0, 0);
	high_score2 = eeprom_read_score(0, 2);
	high_score3 = eeprom_read_score(0, 4);

	// Si la EEPROM es nueva, asignar cero
	if(high_score1 == 0xFFFF) high_score1 = 0;
	if(high_score2 == 0xFFFF) high_score2 = 0;
	if(high_score3 == 0xFFFF) high_score3 = 0;

	// Para reinicio de puntajes:
	//EEPROM_PageErase(0);
	//EEPROM_PageErase(2);
	//EEPROM_PageErase(4);
}

void task_gameplay_statechart(void)
{
    task_gameplay_dta_t *p_task_gameplay_dta;
    p_task_gameplay_dta = &task_gameplay_dta;

    /* 1. ACTUALIZACIÓN DE TIEMPO (TICK) */
    // Fundamental: Si no restamos el tiempo, los delays nunca terminan.
    if (p_task_gameplay_dta->tick > 0)
    {
        p_task_gameplay_dta->tick--;
    }

    /* 2. LECTURA DE EVENTOS */
    if (true == any_event_task_gameplay()) {
        p_task_gameplay_dta->event = get_event_task_gameplay();
        p_task_gameplay_dta->flag = true;
    }

    /* 3. LÓGICA GLOBAL DE BOTONES (FEEDBACK VISUAL) */
    // Esto solo corre si hubo un evento (presionar botón)
    if (p_task_gameplay_dta->flag == true) {
        // Tu lógica original para encender LEDs al pulsar teclas
        if (p_task_gameplay_dta->event == EV_GAME_BTN_RO) {
            put_event_task_actuator(EV_LED_XX_ON, ID_LED_RO);
            p_task_gameplay_dta->color = ROJO;
        }
        if (p_task_gameplay_dta->event == EV_GAME_BTN_VE) {
            put_event_task_actuator(EV_LED_XX_ON, ID_LED_VE);
            p_task_gameplay_dta->color = VERDE;
        }
        if (p_task_gameplay_dta->event == EV_GAME_BTN_AZ) {
            put_event_task_actuator(EV_LED_XX_ON, ID_LED_AZ);
            p_task_gameplay_dta->color = AZUL;
        }
        if (p_task_gameplay_dta->event == EV_GAME_BTN_AM) {
            put_event_task_actuator(EV_LED_XX_ON, ID_LED_AM);
            p_task_gameplay_dta->color = AMARILLO;
        }
        
        // Apagar LEDs si se suelta (IDLE event)
        if (p_task_gameplay_dta->event == EV_GAME_IDLE) {
            // (Tu lógica original de apagado aquí...)
             if(p_task_gameplay_dta -> color == ROJO) put_event_task_actuator(EV_LED_XX_OFF, ID_LED_RO);
             if(p_task_gameplay_dta -> color == VERDE) put_event_task_actuator(EV_LED_XX_OFF, ID_LED_VE);
             if(p_task_gameplay_dta -> color == AZUL) put_event_task_actuator(EV_LED_XX_OFF, ID_LED_AZ);
             if(p_task_gameplay_dta -> color == AMARILLO) put_event_task_actuator(EV_LED_XX_OFF, ID_LED_AM);
             p_task_gameplay_dta -> color = OFF;
        }
    }

    /* 4. MÁQUINA DE ESTADOS PRINCIPAL */
    // ESTA DEBE CORRER SIEMPRE, NO SOLO CUANDO HAY EVENTOS (FLAG)
    switch (p_task_gameplay_dta->state) {

        case ST_GAME_IDLE:
            // En IDLE, solo hacemos cosas si hay eventos (botones)
            if (p_task_gameplay_dta->flag == true)
            {
                if (p_task_gameplay_dta->event == EV_GAME_BTN_RO) put_event_task_menu(EV_MEN_IZQ);
                if (p_task_gameplay_dta->event == EV_GAME_BTN_VE) put_event_task_menu(EV_MEN_ESC);
                if (p_task_gameplay_dta->event == EV_GAME_BTN_AZ) put_event_task_menu(EV_MEN_ENTER);
                if (p_task_gameplay_dta->event == EV_GAME_BTN_AM) put_event_task_menu(EV_MEN_DER);

                if (p_task_gameplay_dta->event == EV_GAME_START_NORMAL)
                {
                    p_task_gameplay_dta->difficulty = 0;
                    p_task_gameplay_dta->score = 0;
                    p_task_gameplay_dta->seq_length = 0;
                    p_task_gameplay_dta->tick = DELAY_GAME_START;
                    p_task_gameplay_dta->state = ST_GAME_INIT_ROUND;
                }
                else if (p_task_gameplay_dta->event == EV_GAME_START_HARD) {
                    p_task_gameplay_dta->difficulty = 1;
                    p_task_gameplay_dta->score = 0;
                    p_task_gameplay_dta->seq_length = 0;
                    p_task_gameplay_dta->tick = DELAY_GAME_START;
                    p_task_gameplay_dta->state = ST_GAME_INIT_ROUND;
                }
                
                // Consumimos el evento
                p_task_gameplay_dta->flag = false;
            }
            break;

        case ST_GAME_INIT_ROUND:
            // Esta lógica corre por TIEMPO, no requiere flag
            if (p_task_gameplay_dta->tick == 0)
            {
            	p_task_gameplay_dta -> sequence[p_task_gameplay_dta -> seq_length] = (rand() % 4) + 1;;
                p_task_gameplay_dta->seq_length++;

                // Lógica de dificultad
                if (p_task_gameplay_dta->difficulty == 0) // Normal
                    p_task_gameplay_dta->play_index = 0;
                else // Dificil
                    p_task_gameplay_dta->play_index = p_task_gameplay_dta->seq_length - 1;

                p_task_gameplay_dta->state = ST_GAME_PLAY_SEQ_ON;
            }
            break;

        case ST_GAME_PLAY_SEQ_ON:
            // Entramos aquí, encendemos led, configuramos tick y nos vamos
            // Nota: Aquí no verificamos tick==0 para entrar, entramos directo por el cambio de estado
            {
                uint8_t color_to_show = p_task_gameplay_dta->sequence[p_task_gameplay_dta->play_index];
                
                if (color_to_show == ROJO) put_event_task_actuator(EV_LED_XX_ON, ID_LED_RO);
                else if (color_to_show == VERDE) put_event_task_actuator(EV_LED_XX_ON, ID_LED_VE);
                else if (color_to_show == AZUL) put_event_task_actuator(EV_LED_XX_ON, ID_LED_AZ);
                else if (color_to_show == AMARILLO) put_event_task_actuator(EV_LED_XX_ON, ID_LED_AM);

                p_task_gameplay_dta->led_on_phase = true; // Estamos encendiendo
                
                // Setear tiempo
                if (p_task_gameplay_dta->difficulty == 0)
                    p_task_gameplay_dta->tick = DELAY_LED_ON_NORMAL;
                else
                    p_task_gameplay_dta->tick = DELAY_LED_ON_HARD;

                p_task_gameplay_dta->state = ST_GAME_PLAY_SEQ_OFF;
            }
            break;

        case ST_GAME_PLAY_SEQ_OFF:
            // Solo actuamos si se consumió el tiempo
            if (p_task_gameplay_dta->tick == 0)
            {
                if (p_task_gameplay_dta->led_on_phase == true)
                {
                    // Apagar todos
                    put_event_task_actuator(EV_LED_XX_OFF, ID_LED_RO);
                    put_event_task_actuator(EV_LED_XX_OFF, ID_LED_VE);
                    put_event_task_actuator(EV_LED_XX_OFF, ID_LED_AZ);
                    put_event_task_actuator(EV_LED_XX_OFF, ID_LED_AM);

                    // Tiempo de pausa
                    p_task_gameplay_dta->tick = (p_task_gameplay_dta->difficulty == 0) ? DELAY_LED_OFF_NORMAL : DELAY_LED_OFF_HARD;
                    p_task_gameplay_dta->led_on_phase = false;
                }
                else // Fin de la pausa
                {
                    p_task_gameplay_dta->play_index++;
                    if (p_task_gameplay_dta->play_index < p_task_gameplay_dta->seq_length) {
                        p_task_gameplay_dta->state = ST_GAME_PLAY_SEQ_ON;
                    } else {
                        // Turno del jugador
                        p_task_gameplay_dta->user_index = 0;
                        
                        // Limpiar cola de eventos viejos
                        while(any_event_task_gameplay()) get_event_task_gameplay();
                        p_task_gameplay_dta->flag = false;
                        
                        p_task_gameplay_dta->state = ST_GAME_WAIT_INPUT;
                    }
                }
            }
            break;

        case ST_GAME_WAIT_INPUT:
            if (p_task_gameplay_dta->flag == true)
            {
                uint8_t pressed_color = OFF;

                // Mapeo directo de Evento -> Color
                switch(p_task_gameplay_dta->event) {
                    case EV_GAME_BTN_RO: pressed_color = ROJO; break;
                    case EV_GAME_BTN_VE: pressed_color = VERDE; break;
                    case EV_GAME_BTN_AZ: pressed_color = AZUL; break;
                    case EV_GAME_BTN_AM: pressed_color = AMARILLO; break;
                    default: pressed_color = OFF; break;
                }

                if (pressed_color != OFF) {
                    p_task_gameplay_dta->last_button_pressed = pressed_color;
                    p_task_gameplay_dta->state = ST_GAME_VERIFY;
                }
                p_task_gameplay_dta->flag = false;
            }
            break;

        case ST_GAME_VERIFY:
             if (p_task_gameplay_dta -> last_button_pressed == p_task_gameplay_dta -> sequence[p_task_gameplay_dta -> user_index])
             {
                 // Correcto
                 p_task_gameplay_dta -> user_index++;
                 if (p_task_gameplay_dta -> user_index >= p_task_gameplay_dta -> seq_length) {
                     p_task_gameplay_dta -> score++;


                     put_event_with_score_task_menu(EV_MEN_UPDATE_SCORE, p_task_gameplay_dta->score);
                     p_task_gameplay_dta -> tick = DELAY_BETWEEN_ROUNDS;
                     p_task_gameplay_dta -> state = ST_GAME_INIT_ROUND;
                 } else {
                     p_task_gameplay_dta -> state = ST_GAME_WAIT_INPUT;
                 }
             } else {
                 p_task_gameplay_dta -> state = ST_GAME_GAME_OVER;
             }
            break;

        case ST_GAME_GAME_OVER:
        {
            uint16_t current_score = p_task_gameplay_dta->score;
            bool new_record = false;

            // 1. Actualización INSTANTÁNEA en RAM (WCET despreciable)
            if (current_score > high_score1) {
                high_score3 = high_score2;
                high_score2 = high_score1;
                high_score1 = current_score;
                new_record = true;
            }
            else if (current_score > high_score2) {
                high_score3 = high_score2;
                high_score2 = current_score;
                new_record = true;
            }
            else if (current_score > high_score3) {
                high_score3 = current_score;
                new_record = true;
            }

            // 2. Si hubo cambios, delegamos el trabajo sucio a Storage
            if (new_record) {
                // ¡Esta línea reemplaza todos tus estados ST_GAME_SAVE_SCORE_x!
                put_event_task_storage(EV_STORAGE_SAVE_SCORES);
            }

            // 3. Feedback inmediato al usuario
            put_event_with_score_task_menu(EV_MEN_GAME_OVER, p_task_gameplay_dta->score);

            // Regresamos a IDLE inmediatamente. Storage guardará en background.
            p_task_gameplay_dta->state = ST_GAME_IDLE;
            break;
        }

         default:
             p_task_gameplay_dta->state = ST_GAME_IDLE;
             break;
    }
}

// ---------------------------------------------------------------------
// 3. Lógica de Gameplay (para actualizar el juego)
// ---------------------------------------------------------------------

void task_gameplay_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_GAME_TICK_CNT_INI < g_task_gameplay_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_gameplay_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_gameplay_cnt++;

		/* Run Task Menu Statechart */
    	task_gameplay_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_GAME_TICK_CNT_INI < g_task_gameplay_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_gameplay_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
	}
}
/********************** end of file ******************************************/
