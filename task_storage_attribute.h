#ifndef TASK_INC_TASK_STORAGE_ATTRIBUTE_H_
#define TASK_INC_TASK_STORAGE_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdbool.h>
#include <stdint.h>

/********************** macros ***********************************************/

/********************** typedef **********************************************/
typedef enum {
    ST_STORAGE_IDLE,

    ST_STORAGE_WRITE_SCORE_1,
    ST_STORAGE_WAIT_1,
    ST_STORAGE_WRITE_SCORE_2,
    ST_STORAGE_WAIT_2,
    ST_STORAGE_WRITE_SCORE_3,
    ST_STORAGE_WAIT_3,

	ST_STORAGE_READ_SCORE_1,
	ST_STORAGE_READ_SCORE_2,
	ST_STORAGE_READ_SCORE_3,
	ST_STORAGE_READ_FINISH,
} task_storage_state_t;


typedef enum {
    EV_STORAGE_IDLE,
    EV_STORAGE_SAVE_SCORES,
	EV_STORAGE_READ_SCORES,
} task_storage_ev_t;


typedef struct {
    uint32_t tick;
    task_storage_state_t state;
    task_storage_ev_t event;
    bool flag;
} task_storage_dta_t;

/********************** external data declaration ****************************/
extern task_storage_dta_t task_storage_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_STORAGE_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
