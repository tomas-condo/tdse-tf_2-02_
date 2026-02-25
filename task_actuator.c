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
 * @file   : task_actuator.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"
#include "task_pwm.h"

/********************** macros and definitions *******************************/
#define G_TASK_ACT_CNT_INIT			0ul
#define G_TASK_ACT_TICK_CNT_INI		0ul

#define DEL_LED_XX_MAX				62ul  // (Original: 250)
#define DEL_LED_XX_PUL				62ul  // (Original: 250)
#define DEL_LED_XX_BLI				125ul // (Original: 500)
#define DEL_LED_XX_MIN				0ul

/********************** internal data declaration ****************************/
const task_actuator_cfg_t task_actuator_cfg_list[] = {
	{ID_LED_AZ,  ACTUATOR_CH_1,   DEL_LED_XX_BLI,  DEL_LED_XX_PUL},
	{ID_LED_RO,  ACTUATOR_CH_0,   DEL_LED_XX_BLI,  DEL_LED_XX_PUL},
	{ID_LED_AM,  ACTUATOR_CH_2,   DEL_LED_XX_BLI,  DEL_LED_XX_PUL},
	{ID_LED_VE,  ACTUATOR_CH_3,   DEL_LED_XX_BLI,  DEL_LED_XX_PUL}
};

#define ACTUATOR_CFG_QTY	(sizeof(task_actuator_cfg_list)/sizeof(task_actuator_cfg_t))

task_actuator_dta_t task_actuator_dta_list[] = {
	{DEL_LED_XX_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false},
	{DEL_LED_XX_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false},
	{DEL_LED_XX_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false},
	{DEL_LED_XX_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false}
};

#define ACTUATOR_DTA_QTY	(sizeof(task_actuator_dta_list)/sizeof(task_actuator_dta_t))

/********************** internal functions declaration ***********************/
void task_actuator_statechart(void);

/********************** internal data definition *****************************/
const char *p_task_actuator 		= "Task Actuator (Actuator Statechart)";
const char *p_task_actuator_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_actuator_cnt;
volatile uint32_t g_task_actuator_tick_cnt;

extern TIM_HandleTypeDef htim3;
extern uint32_t brillo_juego;

static uint32_t g_current_actuator_idx = 0;

/********************** external functions definition ************************/
void task_actuator_init(void *parameters)
{
	uint32_t index;
	const task_actuator_cfg_t *p_task_actuator_cfg;
	task_actuator_dta_t *p_task_actuator_dta;
	task_actuator_st_t state;
	task_actuator_ev_t event;
	bool b_event;

	/* Init & Print out: Task execution counter */
	g_task_actuator_cnt = G_TASK_ACT_CNT_INIT;

	g_current_actuator_idx = 0;

	for (index = 0; ACTUATOR_DTA_QTY > index; index++)
	{
		/* Update Task Actuator Configuration & Data Pointer */
		p_task_actuator_cfg = &task_actuator_cfg_list[index];
		p_task_actuator_dta = &task_actuator_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_LED_XX_OFF;
		p_task_actuator_dta->state = state;

		event = EV_LED_XX_OFF;
		p_task_actuator_dta->event = event;

		b_event = false;
		p_task_actuator_dta->flag = b_event;

		task_pwm_set(p_task_actuator_cfg->pwm_channel, 0);
	}
}

void task_actuator_update(void *parameters)
{
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_actuator_tick_cnt--;
        __asm("CPSIE i");

		g_task_actuator_cnt++;

		/* Run Task Statechart (Solo procesará 1 LED) */
    	task_actuator_statechart();
    }
    else
    {
        __asm("CPSIE i");
    }
}

void task_actuator_statechart(void)
{
	const task_actuator_cfg_t *p_task_actuator_cfg;
	task_actuator_dta_t *p_task_actuator_dta;

    p_task_actuator_cfg = &task_actuator_cfg_list[g_current_actuator_idx];
    p_task_actuator_dta = &task_actuator_dta_list[g_current_actuator_idx];

    switch (p_task_actuator_dta->state)
    {
        case ST_LED_XX_OFF:

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_ON == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, brillo_juego);
                p_task_actuator_dta->state = ST_LED_XX_ON;
            }

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_BLINK == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                p_task_actuator_dta->tick = DEL_LED_XX_MAX; // ~250ms reales (62 * 4)
                task_pwm_set(p_task_actuator_cfg->pwm_channel, brillo_juego);
                p_task_actuator_dta->state = ST_LED_XX_BLINK_ON;
            }

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_PULSE == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                p_task_actuator_dta->tick = DEL_LED_XX_MAX;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, brillo_juego);
                p_task_actuator_dta->state = ST_LED_XX_PULSE;
            }
            break;

        case ST_LED_XX_ON:

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_OFF == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, 0);
                p_task_actuator_dta->state = ST_LED_XX_OFF;
            }
            break;

        case ST_LED_XX_BLINK_ON:
            if(p_task_actuator_dta->tick > DEL_LED_XX_MIN)
            {
                p_task_actuator_dta->tick --;
            }

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_OFF == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, 0);
                p_task_actuator_dta->state = ST_LED_XX_OFF;
            }

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_ON == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, brillo_juego);
                p_task_actuator_dta->state = ST_LED_XX_ON;
            }

            if(p_task_actuator_dta->tick == DEL_LED_XX_MIN)
            {
                p_task_actuator_dta->tick = DEL_LED_XX_MAX;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, 0);
                p_task_actuator_dta->state = ST_LED_XX_BLINK_OFF;
            }
            break;

        case ST_LED_XX_BLINK_OFF:
            if(p_task_actuator_dta->tick > DEL_LED_XX_MIN)
            {
                p_task_actuator_dta->tick --;
            }

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_OFF == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, 0);
                p_task_actuator_dta->state = ST_LED_XX_OFF;
            }

            if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_ON == p_task_actuator_dta->event))
            {
                p_task_actuator_dta->flag = false;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, brillo_juego);
                p_task_actuator_dta->state = ST_LED_XX_ON;
            }

            if(p_task_actuator_dta->tick == DEL_LED_XX_MIN)
            {
                p_task_actuator_dta->tick = DEL_LED_XX_MAX;
                task_pwm_set(p_task_actuator_cfg->pwm_channel, brillo_juego);
                p_task_actuator_dta->state = ST_LED_XX_BLINK_ON;
            }
            break;

        case ST_LED_XX_PULSE:
            if(p_task_actuator_dta->tick > DEL_LED_XX_MIN)
            {
                p_task_actuator_dta->tick --;
            }

            if(p_task_actuator_dta->tick == DEL_LED_XX_MIN)
            {
                task_pwm_set(p_task_actuator_cfg->pwm_channel, 0);
                p_task_actuator_dta->state = ST_LED_XX_OFF;
            }
            break;

        default:
            p_task_actuator_dta->tick  = DEL_LED_XX_MIN;
            p_task_actuator_dta->state = ST_LED_XX_OFF;
            p_task_actuator_dta->event = EV_LED_XX_OFF;
            p_task_actuator_dta->flag = false;
            break;
    }

    g_current_actuator_idx++;
    if (g_current_actuator_idx >= ACTUATOR_DTA_QTY) {
        g_current_actuator_idx = 0;
    }
}
/********************** end of file ******************************************/
