/**
 * @file twi.h
 * @brief Base api for twi.
 *
 * Configuration:
 *   FOSC = 8000000
 *   TWI_FREQUENCY = 100000
 *
 * @author MY Liao
 */

#ifndef _TWI_H_
#define _TWI_H_

/*
 * @addtogroup grp_avr AVR
 * @{
 */

/**
 * @addtogroup grp_i2c I2C
 * @{
 */

#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

void    twi_init( void );
uint8_t	twi_ack( void );
uint8_t twi_start( void );
uint8_t	twi_stop( void );
uint8_t twi_send( uint8_t dat );
uint8_t twi_recv( uint8_t ack );

#endif /*  __AVR_ATmega16__, __AVR_ATmega16A__ */

/* @} */
/* @} */

#endif

