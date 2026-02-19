/********************** inclusions *******************************************/
#include <stdbool.h>

#include "task_storage.h"
#include "task_storage_attribute.h"
#include "task_storage_interface.h"
#include "task_menu_interface.h"
#include "i2c.h"

/********************** macros and definitions *******************************/
#define G_TASK_STORAGE_TICK_CNT_INI	0ul
#define G_TASK_STORAGE_CNT_INI  	0ul

#define DEL_STORAGE_XX_MIN 			0ul
#define STORAGE_WRITE_DELAY_MS  	5
#define STORAGE_READ_DELAY_MS  		1

/********************** internal data declaration ****************************/

task_storage_dta_t task_storage_dta =
{
    /* tick         */ DEL_STORAGE_XX_MIN,
    /* state        */ ST_STORAGE_IDLE,
    /* event        */ EV_STORAGE_IDLE,
    /* flag         */ false
};


#define STORAGE_DTA_QTY	(sizeof(task_storage_dta)/sizeof(task_storage_dta_t))

/********************** internal data definition *****************************/
const char *p_task_storage 		    = "Task Storage";
const char *p_task_storage_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_storage_cnt;
volatile uint32_t g_task_storage_tick_cnt;

extern uint16_t high_score1;
extern uint16_t high_score2;
extern uint16_t high_score3;

task_storage_dta_t task_storage_dta;

/********************** external functions definition ************************/
void task_storage_init(void *parameters)
{
	task_storage_dta_t *p_task_storage_dta;
	task_storage_state_t	state;
	task_storage_ev_t   event;
	bool b_event;

	g_task_storage_cnt = G_TASK_STORAGE_CNT_INI;

	init_queue_event_task_storage();

	p_task_storage_dta = &task_storage_dta;

	state = ST_STORAGE_IDLE;
	p_task_storage_dta->state = state;

	event = EV_STORAGE_IDLE;
	p_task_storage_dta->event = event;

	b_event = false;
	p_task_storage_dta->flag = b_event;
}

void task_storage_statechart(void)
{
    task_storage_dta_t *p_task_storage_dta;
    p_task_storage_dta = &task_storage_dta;

    if (true == any_event_task_storage()) {
        p_task_storage_dta -> flag = true;
        p_task_storage_dta -> event = get_event_task_storage();
    }


    if (p_task_storage_dta->tick > 0)
        p_task_storage_dta->tick--;

    switch (p_task_storage_dta->state)
    {
        case ST_STORAGE_IDLE:
            // En IDLE sí dependemos del flag para despertar
            if (p_task_storage_dta->flag) {
                if (p_task_storage_dta->event == EV_STORAGE_SAVE_SCORES)
                    p_task_storage_dta->state = ST_STORAGE_WRITE_SCORE_1;

                //Manejo de lectura
                else if (p_task_storage_dta->event == EV_STORAGE_READ_SCORES)
                	p_task_storage_dta->state = ST_STORAGE_READ_SCORE_1;

                // Consumimos el evento
                p_task_storage_dta->flag = false;
                p_task_storage_dta->event = EV_STORAGE_IDLE;
            }
            break;

        // --- PASO 1 ---
        case ST_STORAGE_WRITE_SCORE_1:
            // Asegúrate que esta función sea la versión "void" sin delays que definimos antes
            eeprom_write_score(0, 0, high_score1);

            p_task_storage_dta->tick = STORAGE_WRITE_DELAY_MS;
            p_task_storage_dta->state = ST_STORAGE_WAIT_1;
            break;

        case ST_STORAGE_WAIT_1:
            // Ahora sí funcionará porque tick se está decrementando arriba
            if (p_task_storage_dta->tick == 0) {
                p_task_storage_dta->state = ST_STORAGE_WRITE_SCORE_2;
            }
            break;

        // --- PASO 2 ---
        case ST_STORAGE_WRITE_SCORE_2:
            eeprom_write_score(0, 2, high_score2);
            p_task_storage_dta->tick = STORAGE_WRITE_DELAY_MS;
            p_task_storage_dta->state = ST_STORAGE_WAIT_2;
            break;

        case ST_STORAGE_WAIT_2:
            if (p_task_storage_dta->tick == 0) {
                p_task_storage_dta->state = ST_STORAGE_WRITE_SCORE_3;
            }
            break;

        // --- PASO 3 ---
        case ST_STORAGE_WRITE_SCORE_3:
            eeprom_write_score(0, 4, high_score3);
            p_task_storage_dta->tick = STORAGE_WRITE_DELAY_MS;
            p_task_storage_dta->state = ST_STORAGE_WAIT_3;
            break;

        case ST_STORAGE_WAIT_3:
            if (p_task_storage_dta->tick == 0) {
                // Terminado. Volvemos a dormir.
                p_task_storage_dta->state = ST_STORAGE_IDLE;
            }
            break;


        case ST_STORAGE_READ_SCORE_1:
        	// Leemos directo a la variable global
            high_score1 = eeprom_read_score(0, 0);
            // Damos 1 tick de respiro al bus
            p_task_storage_dta->tick = STORAGE_READ_DELAY_MS;
            p_task_storage_dta->state = ST_STORAGE_READ_SCORE_2;
            break;

        case ST_STORAGE_READ_SCORE_2:
        	if (p_task_storage_dta->tick == 0) {
        		high_score2 = eeprom_read_score(0, 2);
                p_task_storage_dta->tick = STORAGE_READ_DELAY_MS;
                p_task_storage_dta->state = ST_STORAGE_READ_SCORE_3;
            }
            break;

        case ST_STORAGE_READ_SCORE_3:
        	if (p_task_storage_dta->tick == 0) {
        		high_score3 = eeprom_read_score(0, 4);
                p_task_storage_dta->state = ST_STORAGE_READ_FINISH;
            }
            break;

            case ST_STORAGE_READ_FINISH:
            	// Avisamos al MENU que los datos globales están actualizados
                put_event_task_menu(EV_MEN_SCORES_UPDATED);
                p_task_storage_dta->state = ST_STORAGE_IDLE;
                break;

        default:
            p_task_storage_dta->state = ST_STORAGE_IDLE;
            break;
    }
}


void task_storage_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_STORAGE_TICK_CNT_INI < g_task_storage_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_storage_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_storage_cnt++;

		/* Run Task Storage Statechart */
    	task_storage_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_STORAGE_TICK_CNT_INI < g_task_storage_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_storage_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
	}
}
