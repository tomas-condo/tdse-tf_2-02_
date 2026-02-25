#ifndef TASK_PWM_H_
#define TASK_PWM_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "main.h"
#include "task_pwm_attribute.h"

/********************** external data declaration ****************************/
extern uint32_t g_task_pwm_cnt;
extern volatile uint32_t g_task_pwm_tick_cnt;

/********************** typedef **********************************************/
void task_pwm_init(void *parameters);

void task_pwm_set(actuator_ch_t channel, uint32_t duty);

void task_pwm_update(void *parameters);

#endif /* TASK_PWM_H_ */
