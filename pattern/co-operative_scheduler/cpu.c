#include <avr/io.h>
#include <avr/interrupt.h>
#include "cpu.h"
#include "scheduler.h"

#define TIMER_INIT	125

void sysclk_init( void ){
	// 设置定时器中断使能
	TIMSK &= 0xc;
	TIMSK |= 0x02;
	// 64分频, 125个定时器时钟为1ms
	OCR0 = 125;
	TCCR0 = 0;
}

void sysclk_start( void ){
	// 64分频, CTC模式
	TCCR0 = 0x0b;
}

void sysclk_stop( void ){
	TCCR0 = 0;
}


ISR( TIMER0_COMP_vect ){
	sch_update();
}
