#include <ioport.h>
#include <stdint.h>
#include <avr/io.h>
#include "ili9325.h"

/* Pin Configuration */
/*  
#define ILI9325_PIN_RS	PORT_A0;
#define ILI9325_PIN_CS	PORT_A1;
#define ILI9325_PIN_WR	PORT_A2;
#define ILI9325_PIN_RD	PORT_A3;
#define ILI9325_PIN_RST	PORT_A4;
#define ILI9325_PIN_DB	PORT_B;
*/


void ili9325_pin_setup( void ){
	iopin_outmode( ILI9325_PIN_RS );
	iopin_outmode( ILI9325_PIN_CS );
	iopin_outmode( ILI9325_PIN_WR );
	iopin_outmode( ILI9325_PIN_RD );
	iopin_outmode( ILI9325_PIN_RST );
	ioport_outmode( ILI9325_PIN_DB );

	iopin_outhigh( ILI9325_PIN_CS );
	iopin_outhigh( ILI9325_PIN_RS );
	iopin_outhigh( ILI9325_PIN_RD );
	iopin_outhigh( ILI9325_PIN_WR );
}

void ili9325_pin_CS_setValue( uint8_t v ){
	iopin_out( ILI9325_PIN_CS, v );
}

void ili9325_pin_WR_setValue( uint8_t v ){
	iopin_out( ILI9325_PIN_WR, v );

}

void ili9325_pin_RD_setValue( uint8_t v ){
	iopin_out( ILI9325_PIN_RD, v );
}

void ili9325_pin_RST_setValue( uint8_t v ){
	iopin_out( ILI9325_PIN_RST, v );
}

void ili9325_pin_DB_setValue( uint8_t v ){
	ioport_out( ILI9325_PIN_DB, v );
}

void ili9325_pin_DB_setOutMode( void ){
	ioport_outmode( ILI9325_PIN_DB );
}

void ili9325_pin_DB_setInMode( void ){
	ioport_inmode( ILI9325_PIN_DB );
}

uint8_t ili9325_pin_DB_getValue( void ){
	return ioport_get( ILI9325_PIN_DB );
}

void ili9325_pin_RS_setValue( uint8_t v ){
	iopin_out( ILI9325_PIN_RS, v );
}
