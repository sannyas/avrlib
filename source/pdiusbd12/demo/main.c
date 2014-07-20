#include <stdint.h>
#include <usart.h>
#include "../pdiusbd12.h"
#include "../isr.h"

uint8_t func_call_chain_cnt;

int main( void ){
	uint16_t temp_16;
	uint8_t temp_8;

	usart_init( );
	stdout = &usart_file;
	printf( "main start\n" );

	d12_init();
	d12_disconnect();
	d12_connect();
	
	temp_16 = d12_getID();
	printf( "%x\n", temp_16 );

	while( 1 ){
		d12_isrHandle();
	}

	printf( "main end\n" );
	while( 1 );
	return 0;
}

