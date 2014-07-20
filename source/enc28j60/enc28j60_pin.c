#include <avr/io.h>
#include <ioport.h>
#include "enc28j60.h"

/*
 * ENC28J60_PIN_CS
 * ENC28J60_PIN_SCK
 * ENC28J60_PIN_SI
 * ENC28J60_PIN_SO
 * ENC28J60_PIN_RESET
 */


void enc28j60_pin_setup(){
	iopin_outmode( ENC28J60_PIN_CS );
	iopin_outmode( ENC28J60_PIN_SCK );
	iopin_outmode( ENC28J60_PIN_SI );
	iopin_inmode( ENC28J60_PIN_SO );
	iopin_outmode( ENC28J60_PIN_RESET );
	
	iopin_outhigh( ENC28J60_PIN_CS );
	iopin_outlow( ENC28J60_PIN_SCK );
	iopin_outhigh( ENC28J60_PIN_RESET );
}

void enc28j60_pin_CS_setValue( uint8_t v ){
	iopin_out( ENC28J60_PIN_CS, v );
}

void enc28j60_pin_SCK_setValue( uint8_t v ){
	iopin_out( ENC28J60_PIN_SCK, v );
}

uint8_t enc28j60_pin_SO_getValue( void ){
	return iopin_get( ENC28J60_PIN_SO );
}

void enc28j60_pin_SI_setValue( uint8_t v ){
	iopin_out( ENC28J60_PIN_SI, v );
}
void enc28j60_pin_RESET_setVakue( uint8_t v ){
	iopin_out( ENC28J60_PIN_RESET, v );
}

