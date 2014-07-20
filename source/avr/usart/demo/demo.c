#include <stdint.h>
#include <stdio.h>
#include <boardled.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "usart.h"

#define BUF_LEN	16

static char buffer[ BUF_LEN ];

int main( void ){
	stdout = &usart_file;
	char ch;
	uint8_t i,j;

	//boardled_init( );
	usart_init();
	while( 1 ){
		printf( "hello, world\n" );
	}

	for( i=0; i<10; ++i ){
		for( j=0; j<10; j++ ){
			usart_putc( j+'0' );
			_delay_ms( 50 );
			//boardled_ch( );
		}
		//usart_putc( '\r' );
		usart_putc( '\n' );
	}

	while( 1 ){
	}
	while( 1 ){
		i = 0;
		while( (ch=usart_getc()) != '\n' )
			buffer[ i++ ] = ch;
		buffer[ i ] = 0;

		i = 0;
		while( (ch=buffer[i++]) != 0 )
			usart_putc( ch );
	}
	return 0;
}
