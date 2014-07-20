#include <avr/io.h>
#include <ioport.h>

#include "ds1302.h"

/* Pin Configuration
 * #define DS1302_PIN_IO	PORT_A0
 * #define DS1302_PIN_RST	PORT_A1
 * #define DS1302_PIN_SCLK	PORT_A2
 */

void ds1302_pin_setUp( void ){
	iopin_outmode( DS1302_PIN_IO );
	iopin_outmode( DS1302_PIN_RST );
	iopin_outmode( DS1302_PIN_SCLK );
}

void ds1302_pin_IO_setOutMode( void ){
	iopin_outmode( DS1302_PIN_IO );
}

void ds1302_pin_IO_setInMode( void ){
	iopin_inmode( DS1302_PIN_IO );
}

void ds1302_pin_IO_set( uint8_t v ){
	iopin_out( DS1302_PIN_IO, v );
}

uint8_t ds1302_pin_IO_get( void ){
	return iopin_get( DS1302_PIN_IO );
}

void ds1302_pin_RST_set( uint8_t v ){
	iopin_out( DS1302_PIN_RST, v );
}

uint8_t ds1302_pin_RST_get( void ){
	return iopin_get( DS1302_PIN_RST );
}

void ds1302_pin_SCLK_set( uint8_t v ){
	iopin_out( DS1302_PIN_SCLK, v );
}

uint8_t ds1302_pin_SCLK_get( void ){
	return iopin_get( DS1302_PIN_SCLK );
}

