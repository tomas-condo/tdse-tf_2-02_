#ifndef TASK_INC_TASK_STORAGE_H_
#define TASK_INC_TASK_STORAGE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_storage_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern uint32_t g_task_storage_cnt;
extern volatile uint32_t g_task_storage_tick_cnt;

/********************** external functions declaration ***********************/
extern void task_storage_init(void *parameters);
extern void task_storage_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_STORAGE_H_ */

/********************** end of file ******************************************/
