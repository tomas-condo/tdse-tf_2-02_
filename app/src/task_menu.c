/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_menu.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"
#include "math.h"
#include "stdio.h"

/* Demo includes */
#include "logger.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_menu.h"
#include "task_menu_attribute.h"
#include "task_menu_interface.h"
#include "task_display.h"
#include "display.h"
#include "task_gameplay.h"
#include "task_gameplay_interface.h"
#include "i2c.h"
#include "task_storage_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI			0ul
#define G_TASK_MEN_TICK_CNT_INI		0ul

#define DEL_MEN_XX_MIN				0ul
#define DEL_MEN_XX_MED				50ul
#define DEL_MEN_XX_MAX				500ul

/********************** internal data declaration ****************************/

task_menu_dta_t task_menu_dta =
{
    /* tick         */ DEL_MEN_XX_MIN,
    /* state        */ ST_MEN_MAIN,
    /* event        */ EV_MEN_IDLE,
    /* flag         */ false,



    // CAMPOS DE ÍNDICE AÑADIDOS
	/* index_menu0  */ 0,
    /* index_menu1  */ 0,
    /* index_menu2  */ 0,

	/* input_score  */ 0,

	/* high_scores  */ {0, 0, 0}
};


#define MENU_DTA_QTY	(sizeof(task_menu_dta)/sizeof(task_menu_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_menu 		= "Task Menu (Interactive Menu)";
const char *p_task_menu_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_menu_cnt;
volatile uint32_t g_task_menu_tick_cnt;

extern uint16_t high_score1;
extern uint16_t high_score2;
extern uint16_t high_score3;

/********************** external functions definition ************************/
void task_menu_init(void *parameters)
{
	task_menu_dta_t *p_task_menu_dta;
	task_menu_state_t	state;
	task_menu_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	/*LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_menu_init), p_task_menu);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_menu), p_task_menu_);*/

	/* Init & Print out: Task execution counter */
	g_task_menu_cnt = G_TASK_MEN_CNT_INI;
	//LOGGER_INFO("   %s = %lu", GET_NAME(g_task_menu_cnt), g_task_menu_cnt);

	init_queue_event_task_menu();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_menu_dta = &task_menu_dta;

	//p_task_menu_dta->high_scores[0] = eeprom_read_score(0, 0); // Top 1
	//p_task_menu_dta->high_scores[1] = eeprom_read_score(0, 2); // Top 2
	//p_task_menu_dta->high_scores[2] = eeprom_read_score(0, 4); // Top 3

	put_event_task_storage(EV_STORAGE_READ_SCORES);

	/* Init & Print out: Task execution FSM */
	state = ST_MEN_MAIN;
	p_task_menu_dta->state = state;

	event = EV_MEN_IDLE;
	p_task_menu_dta->event = event;

	b_event = false;
	p_task_menu_dta->flag = b_event;

	/*LOGGER_INFO(" ");
	LOGGER_INFO("   %s = %lu   %s = %lu   %s = %s",
				 GET_NAME(state), (uint32_t)state,
				 GET_NAME(event), (uint32_t)event,
				 GET_NAME(b_event), (b_event ? "true" : "false"));*/

    displayCharPositionWrite(0, 0);
	displayStringWrite("     Bienvenido     ");

	displayCharPositionWrite(0, 2);
	displayStringWrite("   Pulse un boton   ");
}

void task_menu_statechart(void) {
    task_menu_dta_t *p_task_menu_dta;
    /* Update Task Menu Data Pointer */
    p_task_menu_dta = &task_menu_dta;

    // ---------------------------------------------------------------------
    // 1. Manejo de Eventos
    // ---------------------------------------------------------------------

    if (true == any_event_task_menu()) {
        p_task_menu_dta -> flag = true;
        p_task_menu_dta -> event = get_event_task_menu();
    }

    // ---------------------------------------------------------------------
    // 2. Máquina de Estados (solo si hay un evento para procesar)
    // ---------------------------------------------------------------------
    if (true == p_task_menu_dta -> flag) {
        switch (p_task_menu_dta -> state){

            // =============================================================
            // ESTADO MAIN
            // =============================================================
        case ST_MEN_MAIN:
        	// PASO 1: Mostrar instrucciones (Primera vez que se pulsa botón en Bienvenidos)
            if (p_task_menu_dta->index_menu0 == 0) {
            	task_display_set_line(0,"Instrucciones:      ");
            	task_display_set_line(1,"                    ");
            	task_display_set_line(2,"  Ro=IZQ   Am=DER   ");
            	task_display_set_line(3,"  Ve=ESC  Az=ENTER  ");

                // Marcamos que ya mostramos el texto
                p_task_menu_dta->index_menu0 = 1;

                // Así el sistema se queda esperando el SIGUIENTE botón.
                p_task_menu_dta->flag = false;
            }

            // PASO 2: Esperar segunda pulsación para salir de Instrucciones
            else {
            	if (EV_MEN_ENTER == p_task_menu_dta->event){
                	task_display_set_line(0,"Menu:               ");
                	task_display_set_line(1,"                    ");
                	task_display_set_line(2,">Jugar    Puntajes  ");
                	task_display_set_line(3,"                    ");

                    p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                    p_task_menu_dta -> index_menu1 = 0;

                    // Reseteamos index_menu0 para que si volvemos a este estado, funcione igual
                    p_task_menu_dta -> index_menu0 = 0;

                } else if (EV_MEN_ESC == p_task_menu_dta->event||
                		   EV_MEN_DER == p_task_menu_dta->event||
                           EV_MEN_IZQ == p_task_menu_dta->event){
                	// Si aprietan otro botón, nos quedamos aquí y consumimos el evento
                    p_task_menu_dta -> flag = false;
                    // No cambiamos de estado, seguimos en ST_MEN_MAIN (Instrucciones)
                }
            }
            break;
            // =============================================================
            // ESTADO MENU 1
            // =============================================================
            case ST_MEN_MENU1:

            	/*************************************************/

                if (p_task_menu_dta -> event == EV_MEN_DER){
                    if (p_task_menu_dta -> index_menu1 == 0){
                    	task_display_set_line(0,"Menu:               ");
                    	task_display_set_line(1,"                    ");
                    	task_display_set_line(2," Jugar   >Puntajes  ");
                    	task_display_set_line(3,"                    ");
                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> index_menu1++;
                    }
                    else if (p_task_menu_dta->index_menu1 == 1)
                        p_task_menu_dta -> flag = false;

                    // La transición es a MENU 1 en ambos casos
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                }

                /*************************************************/

                if (p_task_menu_dta -> event == EV_MEN_IZQ){
                    if (p_task_menu_dta -> index_menu1 == 0)
                        p_task_menu_dta -> flag = false;

                    else if (p_task_menu_dta->index_menu1 == 1){
                    	task_display_set_line(2,">Jugar    Puntajes  ");
                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> index_menu1 = 0;
                    }
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                }

                /*************************************************/
                else if (p_task_menu_dta -> event == EV_MEN_ESC){
                	task_display_set_line(0,"Instrucciones:      ");
                	task_display_set_line(1,"                    ");
                	task_display_set_line(2,"  Ro=IZQ  Am=DER  ");
                	task_display_set_line(3,"  Ve=ESC  Az=ENTER  ");

                	p_task_menu_dta -> index_menu0 = 1;

                	p_task_menu_dta -> flag = false;
                	p_task_menu_dta -> state = ST_MEN_MAIN;
                }

                /*************************************************/

                else if (p_task_menu_dta -> event == EV_MEN_ENTER){
                    if (p_task_menu_dta->index_menu1 == 0){
                    	task_display_set_line(0,"Dificultad:         ");
                    	task_display_set_line(1,"                    ");
                    	task_display_set_line(2,">Normal    Dificil  ");
                    	task_display_set_line(3,"                    ");
                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> state = ST_MEN_MENU2;
                        p_task_menu_dta -> index_menu2 = 0;
                        }
                    else if (p_task_menu_dta->index_menu1 == 1){

                    	//p_task_menu_dta->high_scores[0] = eeprom_read_score(0, 0);
                    	//p_task_menu_dta->high_scores[1] = eeprom_read_score(0, 2);
                    	//p_task_menu_dta->high_scores[2] = eeprom_read_score(0, 4);
                    	put_event_task_storage(EV_STORAGE_READ_SCORES);

                    	task_display_set_line(0,"Puntajes:           ");
                    	task_display_printf(1,  "#1 %d", p_task_menu_dta -> high_scores[0]);
                    	task_display_printf(2,  "#2 %d", p_task_menu_dta -> high_scores[1]);
                    	task_display_printf(3,  "#3 %d", p_task_menu_dta -> high_scores[2]);
                        // La transición es a MENU 4
                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> state = ST_MEN_MENU3;
                    }
                }

                if (p_task_menu_dta->event == EV_MEN_SCORES_UPDATED) {
                	p_task_menu_dta->high_scores[0] = high_score1;
                    p_task_menu_dta->high_scores[1] = high_score2;
                    p_task_menu_dta->high_scores[2] = high_score3;
                    p_task_menu_dta->flag = false;
                }

                break;

            case ST_MEN_MENU2: //Mostrando Normal Dificil

                if (p_task_menu_dta -> event == EV_MEN_DER){
                    if (p_task_menu_dta -> index_menu2 == 0){
                    	task_display_set_line(2," Normal   >Dificil  ");
                        p_task_menu_dta -> index_menu2++;
                    }
                    p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> state = ST_MEN_MENU2;
                }

                /*************************************************/

                if (p_task_menu_dta -> event == EV_MEN_IZQ){
                    if (p_task_menu_dta -> index_menu2 == 0)
                        p_task_menu_dta -> flag = false;

                    else if (p_task_menu_dta->index_menu2 == 1){
                    	task_display_set_line(2,">Normal    Dificil  ");
                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> index_menu2 = 0;
                    }
                    // La transición es a MENU 1 en ambos casos
                    p_task_menu_dta -> state = ST_MEN_MENU2;
                }

                /*************************************************/

                if (p_task_menu_dta -> event == EV_MEN_ESC){
                	task_display_set_line(0,"Menu:               ");
                	task_display_set_line(1,"                    ");
                	task_display_set_line(2,">Jugar    Puntajes  ");
                	task_display_set_line(3,"                    ");
                    p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> index_menu1 = 0;
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                }

                /*************************************************/

                if (p_task_menu_dta -> event == EV_MEN_ENTER){
                	p_task_menu_dta->input_score = 0;
                    if (p_task_menu_dta -> index_menu2 == 0){
                    	task_display_set_line(0,"Normal:             ");
                    	task_display_set_line(1,"                    ");
                    	task_display_printf(2,  "Puntaje: %d", p_task_menu_dta->input_score);
                    	task_display_set_line(3,"                    ");

                    	put_event_task_gameplay(EV_GAME_START_NORMAL);

                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> state = ST_MEN_MENU4;
                    }
                    if (p_task_menu_dta -> index_menu2 == 1){
                    	task_display_set_line(0,"Dificil:            ");
                    	task_display_set_line(1,"                    ");
                    	task_display_printf(2,  "Puntaje: %d", p_task_menu_dta->input_score);
                    	task_display_set_line(3,"                    ");

                    	put_event_task_gameplay(EV_GAME_START_HARD);

                        p_task_menu_dta -> flag = false;
                        p_task_menu_dta -> state = ST_MEN_MENU4;
                    }
                }

                break;

            case ST_MEN_MENU3:	//Mostrando puntajes históricos


            	if (p_task_menu_dta->event == EV_MEN_SCORES_UPDATED) {

            	p_task_menu_dta->high_scores[0] = high_score1;
            	p_task_menu_dta->high_scores[1] = high_score2;
            	p_task_menu_dta->high_scores[2] = high_score3;

            	task_display_set_line(0,"Puntajes:           ");
            	task_display_printf(1,  "#1 %d", p_task_menu_dta -> high_scores[0]);
            	task_display_printf(2,  "#2 %d", p_task_menu_dta -> high_scores[1]);
            	task_display_printf(3,  "#3 %d", p_task_menu_dta -> high_scores[2]);

            	p_task_menu_dta->flag = false;
            	}

            	if(p_task_menu_dta -> event == EV_MEN_ESC){
                	task_display_set_line(0,"Menu:               ");
                	task_display_set_line(1,"                    ");
                	task_display_set_line(2,">Jugar    Puntajes  ");
                	task_display_set_line(3,"                    ");
                    p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> index_menu1 = 0;
            		p_task_menu_dta -> state = ST_MEN_MENU1;
            	}

            	if(p_task_menu_dta -> event == EV_MEN_ENTER ||
            	   p_task_menu_dta -> event == EV_MEN_IZQ ||
            	   p_task_menu_dta -> event == EV_MEN_DER)
            	p_task_menu_dta -> state = ST_MEN_MENU3;

            	break;

            case ST_MEN_MENU4:

            	if (p_task_menu_dta->event == EV_MEN_GAME_OVER)
            	{
            		// 2. Leemos el "Buzón" (input_score) y dibujamos
            	    // Nota: task_menu_dta.input_score ya fue actualizado por la primitiva

            	    task_display_set_line(0, "     GAME OVER      ");

            	    // ¡Aquí usamos el dato recibido!
            	    task_display_printf(1,   "Puntaje Final: %d   ", p_task_menu_dta->input_score);

            	    task_display_set_line(2, "                    ");
            	    task_display_set_line(3, "Pulse ESC para salir");

            	    // 3. Transición al estado de visualización de resultado
            	    p_task_menu_dta->flag = false;
            	    p_task_menu_dta->state = ST_MEN_GAME_OVER;
            	}

            	else if (p_task_menu_dta->event == EV_MEN_UPDATE_SCORE)
            	{
            	// Solo actualizamos la línea del puntaje.
            	task_display_printf(2,  "Puntaje: %d", p_task_menu_dta->input_score);
            	p_task_menu_dta->flag = false;
            	//seguimos en ST_MEN_MENU4
            	}

            	break;

            case ST_MEN_GAME_OVER:
            	if(p_task_menu_dta -> event == EV_MEN_IZQ ||
            	   p_task_menu_dta -> event == EV_MEN_DER ||
				   p_task_menu_dta -> event == EV_MEN_ENTER){
            		p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> state = ST_MEN_GAME_OVER;
            	}

            	if(p_task_menu_dta -> event == EV_MEN_ESC){
                	task_display_set_line(0,"Menu:               ");
                	task_display_set_line(1,"                    ");
                	task_display_set_line(2,">Jugar    Puntajes  ");
                	task_display_set_line(3,"                    ");
                    p_task_menu_dta -> flag = false;
            		p_task_menu_dta -> state = ST_MEN_MENU1;
            	}

            	break;

            default:
                p_task_menu_dta->state = ST_MEN_MAIN;
                break;
        }
        p_task_menu_dta->flag = false;
    }
}
// ---------------------------------------------------------------------
// 3. Lógica de Display (para actualizar el LCD)
// ---------------------------------------------------------------------

void task_menu_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_menu_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_menu_cnt++;

		/* Run Task Menu Statechart */
    	task_menu_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_menu_tick_cnt--;
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
