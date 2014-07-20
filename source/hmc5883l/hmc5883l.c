#include <stdint.h>
#include <stdio.h>
#include <i2c.h>
#include <util/delay.h>

#undef AVRLIB_DEBUG
#ifdef HMC5883L_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "hmc5883l.h"

void hmc5883l_init( hmc5883l_configuration_t *conf ){

	DEBUG_FUNC_ENTER();

	if( conf != 0 ){
		hmc5883l_writeConfiguration( conf );
	}
	else{ /* default */
		hmc5883l_configuration_t conf;
		conf.conf_a = 0xF0;
		conf.conf_b = 0x20;
		conf.mode = 0x01;
		hmc5883l_writeConfiguration( &conf );
	}

	DEBUG_FUNC_EXIT();
}

void hmc5883l_readConfiguration( hmc5883l_configuration_t *conf ){
	DEBUG_FUNC_ENTER();

	i2c_readBytes( HMC5883L_SLAVER, HMC5883L_REG_ADDR_CONF_A,
			(uint8_t*)conf, 3);

	DEBUG_FUNC_EXIT();
}

void hmc5883l_writeConfiguration( hmc5883l_configuration_t *conf ){
	DEBUG_FUNC_ENTER();

	i2c_writeBytes( HMC5883L_SLAVER, HMC5883L_REG_ADDR_CONF_A,
			(uint8_t*)conf, 3 );

	DEBUG_FUNC_EXIT();
}

static void __swap_uint16( uint8_t *data_p ){
	uint8_t temp;
	uint8_t *d_p = (uint8_t*)data_p;
	temp = d_p[0];
	d_p[0] = d_p[1];
	d_p[1] = temp;
}

void hmc5883l_getSensorData( hmc5883l_sensor_data_t *data ){
	DEBUG_FUNC_ENTER();

	uint8_t *data_p = (uint8_t *)data;

	i2c_readBytes( HMC5883L_SLAVER, HMC5883L_REG_ADDR_X_MSB,
			data_p, 6 );
	__swap_uint16( data_p );
	__swap_uint16( data_p+2 );
	__swap_uint16( data_p+4 );

	DEBUG_FUNC_EXIT();
}

