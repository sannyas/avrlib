#include "pcf8591.h"
#include <i2c.h>

uint8_t pcf8591_conversion( uint8_t channel ){
	return i2c_readByte( 
			PCF8591_I2C_ADDR,
			channel );
}

void pcf8591_analogOutput( uint8_t value ){
	i2c_writeByte( PCF8591_I2C_ADDR,
			PCF8591_ANALOG_OUTPUT,
			value );
}
