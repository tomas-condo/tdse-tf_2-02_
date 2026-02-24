#ifndef TASK_GAMEPLAY_H_
#define TASK_GAMEPLAY_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "task_gameplay_attribute.h"

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern uint32_t g_task_gameplay_cnt;
extern volatile uint32_t g_task_gameplay_tick_cnt;

/********************** external functions declaration ***********************/
void task_gameplay_init(void *parameters);
void task_gameplay_update(void *parameters);

//void gameplay_start(game_difficulty_t diff);
uint16_t gameplay_get_score(void);
bool gameplay_is_running(void);
bool gameplay_is_finished(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_GAMEPLAY_H_ */

/********************** end of file ******************************************/
