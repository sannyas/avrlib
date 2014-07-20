#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <usart.h>
#include "../nrf24l01p.h"

int main( void ){

	// init uart
	stdout = &usart_file;
	usart_init();

	printf( "main start.\n" );

	nrf24l01p_test_dpl_tx();

	printf( "main end.\n" );	

	while( 1 ){
	}
	return 0;
}
