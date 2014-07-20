#include <avr/io.h>
#include <twi.h>
#include "i2c.h"

#define I2C_ADDR_WRITE(addr)	(addr<<1)
#define I2C_ADDR_READ(addr)	((addr<<1)+1)

#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

uint8_t	i2c_writeByte( uint8_t dev_addr, uint8_t data_addr, uint8_t data ){
	twi_start( );
	twi_send( I2C_ADDR_WRITE(dev_addr) );
	twi_send( data_addr );
	twi_send( data );
	twi_stop( );

	return 0;
}

uint8_t	i2c_writeBytes( uint8_t dev_addr, uint8_t data_addr, uint8_t *data, uint8_t data_len ){
	uint8_t i;
	twi_start( );
	twi_send( I2C_ADDR_WRITE(dev_addr) );
	twi_send( data_addr );
	for( i=0; i<data_len; ++i ){
		twi_send( data[i] );
	}
	twi_stop();

	return 0;
}

uint8_t	i2c_readByte( uint8_t dev_addr, uint8_t data_addr ){
	uint8_t data;

	twi_start( );
	twi_send( I2C_ADDR_WRITE(dev_addr) );
	twi_send( data_addr );
	twi_start( );
	twi_send( I2C_ADDR_READ(dev_addr) );
	data = twi_recv( 0 );
	twi_stop();

	return data;
}

uint8_t	i2c_readBytes( uint8_t dev_addr, uint8_t data_addr, uint8_t *data, uint8_t data_len ){
	uint8_t i;

	twi_start( );
	twi_send( I2C_ADDR_WRITE(dev_addr) );
	twi_send( data_addr );
	twi_start( );
	twi_send( I2C_ADDR_READ(dev_addr) );
	for( i=0; i<data_len-1; ++i ){
		data[i] = twi_recv( 1 );
	}
	data[i] = twi_recv( 0 );
	twi_stop();

	return data_len;
}

#endif /*  __AVR_ATmega16__, __AVR_ATmega16A__ */
