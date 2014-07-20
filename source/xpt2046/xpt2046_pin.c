#include <ioport.h>
#include "xpt2046.h"

/*
#define XPT_PIN_CS 	PORT_A0
#define XPT_PIN_DCLK	PORT_A1
#define XPT_PIN_DIN	PORT_A2
#define XPT_PIN_DOUT	PORT_A3
*/

void xpt2046_pin_setup( void ){
	iopin_outmode( XPT_PIN_CS );
	iopin_outmode( XPT_PIN_DCLK );
	iopin_outmode( XPT_PIN_DIN );
	iopin_inmode( XPT_PIN_DOUT );

	iopin_outhigh( XPT_PIN_CS );
	iopin_outlow( XPT_PIN_DCLK );
	iopin_outlow( XPT_PIN_DIN );

	iopin_inmode( XPT_PIN_PENIRQ );
	/* TODO irq pin */
}

void xpt2046_pin_CS_setValue( uint8_t v ){
	iopin_out( XPT_PIN_CS, v );
}

void xpt2046_pin_DCLK_setValue( uint8_t v ){
	iopin_out( XPT_PIN_DCLK, v );
}

void xpt2046_pin_DIN_setValue( uint8_t v ){
	iopin_out( XPT_PIN_DIN, v );
}

uint8_t xpt2046_pin_DOUT_getValue( void ){
	return iopin_get( XPT_PIN_DOUT );
}

uint8_t xpt2046_pin_PENIRQ_getValue( void ){
	return iopin_get( XPT_PIN_PENIRQ );
}

