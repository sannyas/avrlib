#ifndef _H_SCHEDULER_
#define _H_SCHEDULER_

#include <stdint.h>

#define MAX_TASK	5

typedef void (*task_func_t)(void);
typedef struct Task{
	task_func_t ptask;
	uint16_t delay;
	uint16_t period;
	uint8_t runme;
} task_t;

void    sch_init( void );
void    sch_start( void );
uint8_t sch_addTask( task_func_t ptask, uint16_t delay, uint16_t period );
uint8_t sch_delTask( uint8_t idx );
void    sch_doTask( void );
void    sch_update( void );

#endif
