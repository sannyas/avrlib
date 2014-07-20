/**
 * @file twi.c
 * @brief Library for twi
 * @author MY Liao
 * @date 2014/02/07
 */

#include <avr/io.h>
#include <stdio.h>
#include "twi.h"

#ifdef TWI_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>


#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

/**
 * @name TWBR_PSx
 * @brief Different TWBR value for different prescale x.
 */
/*@{*/
#define TWBR_PS1(baud)	((F_CPU/(baud)-16)/2)
#define TWBR_PS4(baud)	((F_CPU/(baud)-16)/8)
#define TWBR_PS16(baud)	((F_CPU/(baud)-16)/64)
#define TWBR_PS64(baud)	((F_CPU/(baud)-16)/256)
/*@}*/

/*
 * The value of TWBR must greater than or equal to 10
 */
#if TWBR_PS1(TWI_FREQUENCY) >= 10
  #define TWPS_VALUE 0
  #define TWBR_VALUE TWBR_PS1(TWI_FREQUENCY)
#elif TWBR_PS4(TWI_FREQUENCY) >= 10
  #define TWPS_VALUE 1
  #define TWBR_VALUE TWBR_PS4(TWI_FREQUENCY)
#elif TWBR_PS16(TWI_FREQUENCY) >= 10
  #define TWPS_VALUE 2
  #define TWBR_VALUE TWBR_PS16(TWI_FREQUENCY)
#elif TWBR_PS64(TWI_FREQUENCY) >= 10
  #define TWPS_VALUE 3
  #define TWBR_VALUE TWBR_PS64(TWI_FREQUENCY)
#else
  #error "TWI frequency is invalid"
#endif

/** @brief Initialize twi.
 *
 *  This function should be call before using other functon in
 *  this module. In Order to set SCK's frequency, must define macro
 *  TWI_FREQUENCY when compiling.
 *
 *  @return void
 */
void twi_init( void ){
	//TWBR = 30; // set twi baudrate 100K
	TWBR = TWBR_VALUE;
	//TWSR = 0; // TWPS = 0
	TWSR = TWPS_VALUE;
	TWAR = 0;

}

uint8_t twi_ack( void ){
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

	return 1;
}

uint8_t twi_start( void ){

	uint8_t stat;

	TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	//loop_until_bit_is_set(TWCR, TWINT);
	while( !( TWCR & (1<<TWINT) ) );
	stat = TWSR & 0xF8;
	
	return stat;
}

uint8_t twi_stop( void ){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	return 1;
}

/**
 * @brief send a 8-bit data
 * @param data The data that to be sent
 * @return if successful, 1, else 0
 */
uint8_t twi_send( uint8_t data ){
	uint8_t stat;
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	//loop_until_bit_is_set( TWCR, TWINT );
	while( !( TWCR & (1<<TWINT) ) );
	stat = TWSR & 0xF8;

	return stat;
}

/**
 * @brief receive a 8-bit data
 * @param ack If ack equal to 1, Calling this function 
 *  while send a ack before receive real data
 */
uint8_t twi_recv( uint8_t ack ){
	if( ack )
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	else
		TWCR = (1<<TWINT) | (1<<TWEN);
	//loop_until_bit_is_set( TWCR, TWINT );
	while( !( TWCR & (1<<TWINT) ) );
	
	return TWDR;
}

#endif /*  __AVR_ATmega16__, __AVR_ATmega16A__ */
