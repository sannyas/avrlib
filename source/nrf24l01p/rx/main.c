#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <usart.h>
#include "../nrf24l01p.h"


#define BUF_LEN	16


static char buffer[ BUF_LEN ];

int main( void ){

	// init uart
	stdout = &usart_file;
	usart_init_rx();

	printf( "main start.\n" );

	nrf24l01p_test_rx();

	printf( "main end.\n" );	

	while( 1 ){
	}
	return 0;
}
