#include <stdio.h>
#include <twi.h>
#include <usart.h>
#include <util/delay.h>
#include <pcf8591.h>

int main( void ){
	uint8_t data;
	uint8_t idx;

	/* initialize uart */
	usart_init( );
	stdout = &usart_file;
	printf( "main: start\n" );

	/*  initialize i2c */
	twi_init( );
	
	for( idx=0; idx<10; ++idx ){
		data = pcf8591_conversion( PCF8591_CHANNEL_1 );
		printf( "channel 1: %d\n", data );
		data = pcf8591_conversion( PCF8591_CHANNEL_2 );
		printf( "channel 2: %d\n", data );
		data = pcf8591_conversion( PCF8591_CHANNEL_3 );
		printf( "channel 3: %d\n", data );
		data = pcf8591_conversion( PCF8591_CHANNEL_4 );
		printf( "channel 4: %d\n", data );

		_delay_ms( 1000 );
	}

	while( 1 ){
		for( idx=0; idx<255; ++idx ){
			pcf8591_analogOutput( idx );
			_delay_ms( 10 );
		}
		for( idx=255; idx>0; --idx ){
			pcf8591_analogOutput( idx );
			_delay_ms( 10 );
		}
	}
	printf( "main: end\n" );

	return 0;
}
