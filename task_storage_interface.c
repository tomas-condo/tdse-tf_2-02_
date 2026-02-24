/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"
#include <stdbool.h>

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_storage_attribute.h"

/********************** macros and definitions *******************************/
#define EVENT_UNDEFINED	(255)
#define MAX_EVENTS		(16)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	task_storage_ev_t	queue[MAX_EVENTS];
} queue_task_s;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_task_storage(void)
{
	uint32_t i;

	queue_task_s.head = 0;
	queue_task_s.tail = 0;
	queue_task_s.count = 0;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_task_s.queue[i] = EVENT_UNDEFINED;
}

task_storage_ev_t get_event_task_storage(void)
{
	task_storage_ev_t event;

	queue_task_s.count--;
	event = queue_task_s.queue[queue_task_s.tail];
	queue_task_s.queue[queue_task_s.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_task_s.tail)
		queue_task_s.tail = 0;

	return event;
}

bool any_event_task_storage(void)
{
  return (queue_task_s.head != queue_task_s.tail);
}

void put_event_task_storage(task_storage_ev_t event)
{
	queue_task_s.count++;
	queue_task_s.queue[queue_task_s.head++] = event;

	if (MAX_EVENTS == queue_task_s.head)
		queue_task_s.head = 0;
}

/********************** end of file ******************************************/
