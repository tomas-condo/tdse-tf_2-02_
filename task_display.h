#ifndef TASK_DISPLAY_H_
#define TASK_DISPLAY_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include "task_display_attribute.h"
/********************** macros ***********************************************/
#define DISPLAY_ROWS 4
#define DISPLAY_COLS 20

/********************** external data declaration ****************************/
extern uint32_t g_task_display_cnt;
extern volatile uint32_t g_task_display_tick_cnt;

/********************** external functions declaration ***********************/
void task_display_init(void *parameters);
void task_display_update(void *parameters);

void task_display_set_line(uint8_t row, const char *str);
void task_display_printf(uint8_t row, const char *fmt, ...);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_DISPLAY_H_ */
