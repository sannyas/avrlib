#include <avr/io.h>
#include <ioport.h>
#include "nrf24l01.h"

/*
 * Configuration :
 * 
 * NRF24L01_PIN_CE
 * NRF24L01_PIN_CSN
 * NRF24L01_PIN_SCK
 * NRF24L01_PIN_MOSI
 * NRF24L01_PIN_MISO
 */

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
	iopin_outmode( NRF24L01_PIN_CE );
	iopin_outmode( NRF24L01_PIN_CSN );
	iopin_outmode( NRF24L01_PIN_SCK );
	iopin_outmode( NRF24L01_PIN_MOSI );
	iopin_inmode ( NRF24L01_PIN_MISO );
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
	iopin_out( NRF24L01_PIN_CE, state );
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
	iopin_out( NRF24L01_PIN_CSN, state );
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
	iopin_out( NRF24L01_PIN_SCK, state );
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
	iopin_out( NRF24L01_PIN_MOSI, state );
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return iopin_get( NRF24L01_PIN_MISO );
}
/* ------------------------------------------------------------------------- */
