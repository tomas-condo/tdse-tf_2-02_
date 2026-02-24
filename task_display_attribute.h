#ifndef TASK_INC_TASK_DISPLAY_ATTRIBUTE_H_
#define TASK_INC_TASK_DISPLAY_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>

/********************** macros ***********************************************/
#define DISPLAY_ROWS 4
#define DISPLAY_COLS 20

/********************** typedef **********************************************/
typedef enum {
    ST_DISP_IDLE,
    ST_DISP_SET_CURSOR,
    ST_DISP_WRITE_CHAR
} task_display_state_t;

typedef struct
{
    task_display_state_t state;

    char screen_buffer[DISPLAY_ROWS][DISPLAY_COLS + 1];

    uint8_t curr_row;
    uint8_t curr_col;

    bool dirty_rows[DISPLAY_ROWS];

} task_display_dta_t;

/********************** external data declaration ****************************/
extern task_display_dta_t task_display_dta;

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_DISPLAY_ATTRIBUTE_H_ */
/********************** end of file ******************************************/
