#ifndef TASK_STORAGE_INTERFACE_H_
#define TASK_STORAGE_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_storage_attribute.h"

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_task_storage(void);
void put_event_task_storage(task_storage_ev_t event);
extern task_storage_ev_t get_event_task_storage(void);
extern bool any_event_task_storage(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_STORAGE_INTERFACE_H_ */

/********************** end of file ******************************************/
