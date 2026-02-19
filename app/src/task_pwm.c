/********************** inclusions *******************************************/
#include <stdbool.h>
#include "task_pwm.h"
#include "task_pwm_attribute.h"
#include "task_pwm_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_PWM_CNT_INI      0ul
#define G_TASK_PWM_TICK_CNT_INI 0ul
#define DEL_PWM_XX_MIN 			0ul

#define STEP (2048)
#define PERIOD (65535)

/********************** internal data declaration ****************************/

task_pwm_dta_t task_pwm_dta =
{
    /* tick         */ DEL_PWM_XX_MIN,
    /* state        */ ST_PWM_INIT,
    /* flag         */ false,
	/* Duty Cycles	*/ {0}
};

#define PWM_DTA_QTY	(sizeof(task_pwm_dta)/sizeof(task_pwm_dta_t))

/********************** external data declaration *****************************/
extern TIM_HandleTypeDef htim3;

uint32_t g_task_pwm_cnt;
volatile uint32_t g_task_pwm_tick_cnt;

/********************** external functions definition ************************/
void task_pwm_init(void *parameters)
{
    task_pwm_dta_t *p_task_pwm_dta;
    p_task_pwm_dta = &task_pwm_dta;

    // 1. Inicializar estructuras
    for(int i = 0; i < PWM_CH_MAX; i++)
        task_pwm_dta.duty_cycles[i] = 0;

    g_task_pwm_cnt = G_TASK_PWM_CNT_INI;
    p_task_pwm_dta->flag = false;

    // 2. CAMBIO: Inicializar Hardware AQUÍ (antes del while(1))
    // Esto se ejecuta una vez y no afecta el WCET del bucle principal
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    // 3. CAMBIO: Arrancar directamente en IDLE
    p_task_pwm_dta->state = ST_PWM_IDLE;
}

void task_pwm_set(actuator_ch_t channel, uint32_t duty)
{
    if (channel < PWM_CH_MAX) {
        task_pwm_dta.duty_cycles[channel] = duty;
        task_pwm_dta.flag = true;
    }
}


void task_pwm_statechart(void)
{
    switch (task_pwm_dta.state)
    {
        // El caso ST_PWM_INIT ya no es necesario aquí.

        case ST_PWM_IDLE:
            if (task_pwm_dta.flag == true) {
                // Esto tarda menos de 5us (escribir registros directos)
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, task_pwm_dta.duty_cycles[ACTUATOR_CH_0]);
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, task_pwm_dta.duty_cycles[ACTUATOR_CH_1]);
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, task_pwm_dta.duty_cycles[ACTUATOR_CH_2]);
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, task_pwm_dta.duty_cycles[ACTUATOR_CH_3]);

                task_pwm_dta.flag = false;
            }
            break;

        default:
            task_pwm_dta.state = ST_PWM_IDLE;
            break;
    }
}


void task_pwm_update(void *parameters)
{
    __asm("CPSID i");   /* Deshabilitar interrupciones */

    if (G_TASK_PWM_TICK_CNT_INI < g_task_pwm_tick_cnt)
    {
        g_task_pwm_tick_cnt--;

        __asm("CPSIE i");

        g_task_pwm_cnt++;

        task_pwm_statechart();
    }
    else
    {
        __asm("CPSIE i");
    }
}

