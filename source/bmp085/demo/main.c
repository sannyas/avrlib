#include <stdio.h>
#include <twi.h>
#include <usart.h>
#include <boardled.h>
#include <bmp085.h>
#include <util/delay.h>

int main( void ){
	bmp085_sensor_data_t sdata;

	/* initialize uart */
	usart_init( );
	stdout = &usart_file;
	printf( "main: start\n" );

	/*  initialize i2c */
	twi_init( );
	bmp085_init();
	
	while( 1 ){
		bmp085_getSensorData( &sdata, BMP085_MODE_ULTRA_HIGH_RESOLUTION );
		printf( "temperature is %ld\n", sdata.temperature );
		printf( "pressure is %ld\n", sdata.pressure );
		//printf( "altitude is %ld\n", bmp085_getAltitude( sdata.pressure ) );
		_delay_ms( 1000 );
	}
	printf( "main: end\n" );

	return 0;
}
