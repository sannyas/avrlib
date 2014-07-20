#include <stdio.h>
#include <usart.h>
#include <stdint.h>

void task1_init( void ){
	usart_init();
	stdout = &usart_file;

	printf( "task1 init has completed\n" );
}

static uint32_t cnt;
void task1_do( void ){
	printf( "task1. %ld\n", cnt++ );
}
