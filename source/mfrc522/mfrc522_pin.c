#include <avr/io.h>
#include <ioport.h>
#include <stdio.h>

#include "mfrc522.h"

/*
#define MFRC522_PIN_NSS PORT_A0
#define MFRC522_PIN_SCLK PORT_A1
#define MFRC522_PIN_MOSI PORT_A2
#define MFRC522_PIN_MISO PORT_A3
#define MFRC522_PIN_RST PORT_A4
*/

void mfrc522_pin_init( void ){
	iopin_outmode( MFRC522_PIN_NSS );
	iopin_outmode( MFRC522_PIN_SCLK );
	iopin_outmode( MFRC522_PIN_MOSI );
	iopin_inmode( MFRC522_PIN_MISO );
	iopin_outmode( MFRC522_PIN_RST );

	iopin_outhigh( MFRC522_PIN_NSS );
	iopin_outlow( MFRC522_PIN_SCLK );
	iopin_outhigh( MFRC522_PIN_MOSI );
}

void mfrc522_pin_NSS_set( uint8_t v ){
	iopin_out( MFRC522_PIN_NSS, v );
}

void mfrc522_pin_SCLK_set( uint8_t v ){
	iopin_out( MFRC522_PIN_SCLK, v );
}

void mfrc522_pin_MOSI_set( uint8_t v ){
	iopin_out( MFRC522_PIN_MOSI, v );
}

uint8_t mfrc522_pin_MISO_get( void ){
	return iopin_get( MFRC522_PIN_MISO );
}

void mfrc522_pin_RST_set( uint8_t v ){
	iopin_out( MFRC522_PIN_RST, v );
}

