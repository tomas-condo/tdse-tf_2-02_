
/********************** inclusions *******************************************/
#include "main.h"
#include "logger.h"
#include "dwt.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "task_display.h"
#include "task_display_attribute.h"
#include "display.h"

/********************** macros and definitions *******************************/
#define G_TASK_DIS_CNT_INI      0ul
#define G_TASK_DIS_TICK_CNT_INI 0ul

#define INITROW 0
#define INITCOL 0

/********************** internal data declaration ****************************/
task_display_dta_t task_display_dta =
{
    /* state          */ ST_DISP_IDLE,

    // Inicializamos el buffer con ceros (seguridad).
    // La función task_display_init() se encargará luego de llenarlo con espacios ' '.
    /* screen_buffer  */ {{0}},

    /* curr_row       */ INITROW,
    /* curr_col       */ INITCOL,

    /* flag */ 			 {false,false,false,false}
};

#define DISPLAY_DTA_QTY	(sizeof(task_display_dta)/sizeof(task_display_dta_t))

/********************** internal data definition *****************************/
const char *p_task_display 		= "Task Display";
const char *p_task_display_ 	= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_display_cnt;
volatile uint32_t g_task_display_tick_cnt;

/********************** external functions definition ************************/
void task_display_init(void *parameters)
{
	task_display_dta_t *p_task_display_dta;
	task_display_state_t state;
	bool b_event;

	/* Print out: Task Initialized */
	/*LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_display_init), p_task_display);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_display), p_task_display_);*/

	/* Init & Print out: Task execution counter */
	g_task_display_cnt = G_TASK_DIS_CNT_INI;
	//LOGGER_INFO("   %s = %lu", GET_NAME(g_task_display_cnt), g_task_display_cnt);

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_display_dta = &task_display_dta;

	/* Init & Print out: LCD Display */
	state = ST_DISP_IDLE;
    p_task_display_dta->state = state;

    for(int i=0; i<DISPLAY_ROWS; i++)
    	p_task_display_dta->dirty_rows[i] = false;

	displayInit( DISPLAY_CONNECTION_GPIO_4BITS );

	/*LOGGER_INFO(" ");
	LOGGER_INFO("   %s = %lu   %s = %s",
				 GET_NAME(state), (uint32_t)state,
				 GET_NAME(b_event), (b_event ? "true" : "false"));*/
}

void task_display_set_line(uint8_t row, const char *str)
{
    if (row >= DISPLAY_ROWS) return;

    char temp_buffer[DISPLAY_COLS];
    int i;

    for(i = 0; i < DISPLAY_COLS; i++) {
        if(str[i] != '\0') {
            temp_buffer[i] = str[i];
        } else {
            break;
        }
    }
    for(; i < DISPLAY_COLS; i++) { temp_buffer[i] = ' '; }

    if (memcmp(task_display_dta.screen_buffer[row], temp_buffer, DISPLAY_COLS) == 0)
        return;

    __asm("CPSID i");

    memcpy(task_display_dta.screen_buffer[row], temp_buffer, DISPLAY_COLS);

    task_display_dta.dirty_rows[row] = true;

    __asm("CPSIE i");
}

void task_display_printf(uint8_t row, const char *fmt, ...)
{
    if (row >= DISPLAY_ROWS) return;

    // Buffer temporal local
    char local_buffer[DISPLAY_COLS + 1];

    va_list args;
    va_start(args, fmt);

    vsnprintf(local_buffer, sizeof(local_buffer), fmt, args);

    va_end(args);

    // Llamamos a set_line optimizado
    task_display_set_line(row, local_buffer);
}

// --- Máquina de Estados (Se llama cada 1ms) ---

void task_display_statechart(void){
    task_display_dta_t *p_task_display_dta;
    /* Update Task Display Data Pointer */
    p_task_display_dta = &task_display_dta;

    switch (p_task_display_dta->state)
    {
    case ST_DISP_IDLE:
                for (int i = 0; i < DISPLAY_ROWS; i++) {
                    if (p_task_display_dta->dirty_rows[i] == true) {

                        p_task_display_dta->curr_row = i; // Usar 'i', no '0'
                        p_task_display_dta->curr_col = 0;
                        p_task_display_dta->state = ST_DISP_SET_CURSOR;
                        return;
                    }
                }
                break;


        case ST_DISP_SET_CURSOR:
            displayCharPositionWrite(0, p_task_display_dta->curr_row);

            p_task_display_dta->state = ST_DISP_WRITE_CHAR;
            break;


        case ST_DISP_WRITE_CHAR:
            // Escribimos UN solo caracter
        	displayCharWrite(p_task_display_dta->screen_buffer[p_task_display_dta->curr_row][p_task_display_dta->curr_col]);

            p_task_display_dta->curr_col++;

            if (p_task_display_dta->curr_col >= DISPLAY_COLS) {
            	p_task_display_dta->dirty_rows[p_task_display_dta->curr_row] = false;
            	p_task_display_dta->state = ST_DISP_IDLE;
            }
            break;

        default:
        	p_task_display_dta->state = ST_DISP_IDLE;
            break;
    }
}

void task_display_update(void *parameters)
{
    __asm("CPSID i");

    if (G_TASK_DIS_TICK_CNT_INI < g_task_display_tick_cnt)
    {
        g_task_display_tick_cnt--;

        __asm("CPSIE i");

        g_task_display_cnt++;

        task_display_statechart();
    }
    else
    {
        __asm("CPSIE i");
    }
}
