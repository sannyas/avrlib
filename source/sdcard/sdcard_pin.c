#include <stdint.h>
#include <ioport.h>
#include "sdcard.h"

#define SD_PIN_CS	PORT_A0
#define SD_PIN_DI	PORT_A1
#define SD_PIN_SCLK	PORT_A2
#define SD_PIN_DO	PORT_A3

void sd_pin_setup( void ){
	iopin_outhigh( SD_PIN_CS );	
	iopin_outlow( SD_PIN_SCLK );
	iopin_outhigh( SD_PIN_DI );

	iopin_outmode( SD_PIN_CS );
	iopin_outmode( SD_PIN_SCLK );
	iopin_outmode( SD_PIN_DI );
	iopin_inmode( SD_PIN_DO );
}

void sd_pin_SCLK_setValue( uint8_t v ){
	iopin_out( SD_PIN_SCLK, v );
}

void sd_pin_CS_setValue( uint8_t v ){
	iopin_out( SD_PIN_CS, v );
}

void sd_pin_DI_setValue( uint8_t v ){ /* MOSI */
	iopin_out( SD_PIN_DI, v );
}

uint8_t sd_pin_DO_getValue( void ){ /* MISO */
	return iopin_get( SD_PIN_DO );
}

