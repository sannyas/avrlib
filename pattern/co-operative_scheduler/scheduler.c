#include <avr/interrupt.h>
#include "scheduler.h"
#include "cpu.h"

static task_t tasks[ MAX_TASK ];

void sch_init( void ){
	// init system timer
	sysclk_init();
}

void sch_start( void ){
	// run timer
	sei();
	sysclk_start();
}

uint8_t sch_addTask( task_func_t ptask, uint16_t delay, uint16_t period ){
	uint8_t idx;

	// 搜索任务队列,找到可用的任务
	for( idx=0; idx<MAX_TASK; ++idx ){
		if( tasks[idx].ptask == 0 )
			break;
	}

	if( idx == MAX_TASK ){
		return 255; // TODO 
	}

	tasks[idx].ptask = ptask;
	tasks[idx].delay = delay;
	tasks[idx].period = period;
	tasks[idx].runme = 0;

	printf( "add success\n" );
	return idx;
}

uint8_t sch_delTask( uint8_t idx ){
	if( tasks[idx].ptask == 0 ){
		return 255; // TODO
	}

	tasks[idx].ptask = 0;
	tasks[idx].delay = 0;
	tasks[idx].period = 0;
	tasks[idx].runme = 0;

	return 0;
}

void sch_update( void ){
	uint8_t idx;
	for( idx=0; idx< MAX_TASK; ++idx ){
		if( tasks[idx].ptask == 0 )
			continue;
		if( tasks[idx].delay == 0 ){
			tasks[idx].runme ++;
			if( tasks[idx].period ){
				tasks[idx].delay = tasks[idx].period;
			}
		}
		else{
			tasks[idx].delay--;
		}
	}
}

void sch_doTask( void ){
	uint8_t idx;
	
	for( idx=0; idx<MAX_TASK; ++idx ){
		if( tasks[idx].runme > 0 ){
			(*(tasks[idx].ptask))();
			tasks[idx].runme--;
			if( tasks[idx].period == 0 )
				sch_delTask( idx );
		}
	}
}

