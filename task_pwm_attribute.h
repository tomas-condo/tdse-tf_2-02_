#ifndef TASK_PWM_ATTRIBUTE_H_
#define TASK_PWM_ATTRIBUTE_H_

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"


typedef enum {
    ST_PWM_INIT,
    ST_PWM_IDLE,
} task_pwm_state_t;


/*typedef enum {
    PWM_CH_0 = 0, // Mapeado a TIM_CHANNEL_1 (ROJO)
    PWM_CH_1,     // Mapeado a TIM_CHANNEL_2 (AZUL)
    PWM_CH_2,     // Mapeado a TIM_CHANNEL_3 (AMARILLO)
    PWM_CH_3,     // Mapeado a TIM_CHANNEL_4 (VERDE)
    PWM_CH_MAX
} pwm_channel_t;*/

typedef enum {
    ACTUATOR_CH_0 = 0,
    ACTUATOR_CH_1,
    ACTUATOR_CH_2,
    ACTUATOR_CH_3,
	PWM_CH_MAX
} actuator_ch_t;


// Estructura de datos de la tarea
typedef struct {
    uint32_t tick;
    task_pwm_state_t state;
    bool             flag;           			// Indica si hay cambios pendientes
    uint32_t         duty_cycles[PWM_CH_MAX]; 	// Buffer de valores (0 a 65535 o ARR)
} task_pwm_dta_t;

extern task_pwm_dta_t task_pwm_dta;

#endif /* TASK_PWM_ATTRIBUTE_H_ */
