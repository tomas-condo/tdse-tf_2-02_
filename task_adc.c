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
 * @file   : task_adc.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"
//#include "stm32f1xx_hal_adc.h"
/* Demo includes. */
#include "logger.h"
#include "dwt.h"
//#include "task_adc.h"
/* Application & Tasks includes. */
#include "board.h"
#include "app.h"

/********************** macros and definitions *******************************/
#define ADC_REFRESH_RATE_MS  200

/********************** internal data declaration ****************************/
// Puntero estático para acceder a los datos compartidos desde la ISR (Interrupción)
static shared_data_type *p_adc_shared_data = NULL;

// Flag para evitar reentrancia o iniciar conversión si ya hay una en curso
static volatile bool adc_is_converting = false;

// Variables para control de tiempo (Frecuencia de muestreo)
static uint32_t adc_last_tick = 0;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration *****************************/

extern ADC_HandleTypeDef hadc1;

/********************** external functions definition ************************/
void task_adc_init(void *parameters)
{
	    p_adc_shared_data = (shared_data_type *) parameters;

	    p_adc_shared_data->adc_end_of_conversion = false;

	    adc_is_converting = false;
	    adc_last_tick = HAL_GetTick();
}

void task_adc_update(void *parameters)
{
	if ((HAL_GetTick() - adc_last_tick) >= ADC_REFRESH_RATE_MS)
	{
		if (adc_is_converting == false)
	    {
			if (HAL_ADC_Start_IT(&hadc1) == HAL_OK)
	        {
				adc_is_converting = true;
	            adc_last_tick = HAL_GetTick(); // Reiniciamos timer
	        }
	    }
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        if (p_adc_shared_data != NULL)
        {
            p_adc_shared_data->adc_value = HAL_ADC_GetValue(hadc);

            p_adc_shared_data->adc_end_of_conversion = true;
        }
        adc_is_converting = false;
    }
}

/********************** end of file ******************************************/
