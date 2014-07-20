#include <stdio.h>
#include <usart.h>
#include <stdint.h>

#include "task2.h"

void task2_init( void ){
	printf( "task1 init has completed\n" );
}

static uint32_t cnt;
void task2_do( void ){
	printf( "task2. %ld\n", cnt++ );
}
