#ifndef TASK_INC_TASK_GAMEPLAY_INTERFACE_H_
#define TASK_INC_TASK_GAMEPLAY_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdbool.h>
#include <stdint.h>

#include "task_gameplay_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_task_gameplay(void);
extern void put_event_task_gameplay(task_gameplay_ev_t event);
extern task_gameplay_ev_t get_event_task_gameplay(void);
extern bool any_event_task_gameplay(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_GAMEPLAY_INTERFACE_H_ */

/********************** end of file ******************************************/
