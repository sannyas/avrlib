#include <ioport.h>
#include "pdiusbd12.h"

#define PDIUSBD12_PIN_CS	PORT_B0
#define PDIUSBD12_PIN_A0	PORT_B1
#define PDIUSBD12_PIN_WR	PORT_B2
#define PDIUSBD12_PIN_RD	PORT_B3
#define PDIUSBD12_PIN_INT	PORT_B4
#define PDIUSBD12_PORT_DATA	PORT_A

void d12_pin_setup( void ){
	iopin_outhigh( PDIUSBD12_PIN_CS );
	iopin_outhigh( PDIUSBD12_PIN_A0 );
	iopin_outhigh( PDIUSBD12_PIN_WR );
	iopin_outhigh( PDIUSBD12_PIN_RD );

	iopin_outmode( PDIUSBD12_PIN_CS );
	iopin_outmode( PDIUSBD12_PIN_A0 );
	iopin_outmode( PDIUSBD12_PIN_WR );
	iopin_outmode( PDIUSBD12_PIN_RD );
	iopin_inmode( PDIUSBD12_PIN_INT );
	ioport_outmode( PDIUSBD12_PORT_DATA );
}

void d12_pin_CS_setValue( uint8_t v ){
	iopin_out( PDIUSBD12_PIN_CS, v );
}

void d12_pin_A0_setValue( uint8_t v ){
	iopin_out( PDIUSBD12_PIN_A0, v );
}

void d12_pin_WR_setValue( uint8_t v ){
	iopin_out( PDIUSBD12_PIN_WR, v );
}

void d12_pin_RD_setValue( uint8_t v ){
	iopin_out( PDIUSBD12_PIN_RD, v );
}

void d12_port_DATA_setValue( uint8_t v ){
	ioport_out( PDIUSBD12_PORT_DATA, v );
}

void d12_port_DATA_setOutMode( void ){
	ioport_outmode( PDIUSBD12_PORT_DATA );
}

void d12_port_DATA_setInMode( void ){
	ioport_inmode( PDIUSBD12_PORT_DATA );
}

uint8_t d12_port_DATA_getValue( void ){
	return ioport_get( PDIUSBD12_PORT_DATA );
}

uint8_t d12_pin_INT_getValue( void ){
	return iopin_get( PDIUSBD12_PIN_INT );
}
