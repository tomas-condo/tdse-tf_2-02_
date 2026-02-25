#ifndef TASK_INC_TASK_PWM_INTERFACE_H_
#define TASK_INC_TASK_PWM_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "task_pwm_attribute.h"
/********************** external functions declaration ***********************/
void task_pwm_set_duty(actuator_ch_t channel, uint32_t duty);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_PWM_INTERFACE_H_ */

/********************** end of file ******************************************/
