#include "scheduler.h"
#include <usart.h>
#include "led.h"
#include <util/delay.h>
#include <stdio.h>

int main( void ){

	usart_init();
	stdout = &usart_file;
	printf( "main start\n" );

	sch_init();

	// Initialize task
	led_init();

	// Add task
	sch_addTask( led_do,0,1000 );
	printf( "add complete\n" );

	// start scheduler
	sch_start();
	printf( "h" );

	while( 1 ){
		printf( "k\n" );
		sch_doTask();
	}
}
