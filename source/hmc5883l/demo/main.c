#include <stdio.h>
#include <util/delay.h>
#include <usart.h>
#include <hmc5883l.h>
#include <twi.h>
#include <i2c.h>

int main( void ){
	hmc5883l_sensor_data_t sdata;

	usart_init( );
	stdout = &usart_file;
	printf( "main start\n" );

	twi_init();

	hmc5883l_init( 0 );
	while( 1 ){

		/* get data */
		hmc5883l_getSensorData( &sdata );

		/* output */
		printf( "x = %d\n", sdata.x );
		printf( "y = %d\n", sdata.y );
		printf( "z = %d\n", sdata.z );

		_delay_ms( 1000 );
	}


	return 0;
}
