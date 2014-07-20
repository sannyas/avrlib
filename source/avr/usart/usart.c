/**
 * @file usart.c
 *
 * The detail description \ref grp_usart "hehe"
 */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "usart.h"


#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

#define USART_PARITYBIT_NONE 0
#define USART_PARITYBIT_EVEN 2
#define USART_PARITYBIT_ODD  3
#define __USART_PARITYBIT_VALUE(v) (USART_PARITYBIT_##v)
#define USART_PARITYBIT_VALUE(v) (__USART_PARITYBIT_VALUE(v))

#ifndef BAUD_TOL
#  define BAUD_TOL 2
#endif

#define UBRR_VALUE (((F_CPU) + 8UL * (USART_BAUD)) / (16UL * (USART_BAUD)) -1UL)

#if 100 * (F_CPU) > \
  	(16 * ((UBRR_VALUE) + 1)) * (100 * (USART_BAUD) + (USART_BAUD) * (BAUD_TOL))
  #define USE_2X 1
#elif 100 * (F_CPU) < \
	(16 * ((UBRR_VALUE) + 1)) * (100 * (USART_BAUD) - (USART_BAUD) * (BAUD_TOL))
  #define USE_2X 1
#else
  #define USE_2X 0
#endif

#if USE_2X
  #undef UBRR_VALUE
  #define UBRR_VALUE (((F_CPU) + 4UL * (USART_BAUD)) / (8UL * (USART_BAUD)) -1UL)

  #if 100 * (F_CPU) > \
  (8 * ((UBRR_VALUE) + 1)) * (100 * (USART_BAUD) + (USART_BAUD) * (BAUD_TOL))
    #warning "Baud rate achieved is higher than allowed"
  #endif

  #if 100 * (F_CPU) < \
	(8 * ((UBRR_VALUE) + 1)) * (100 * (USART_BAUD) - (USART_BAUD) * (BAUD_TOL))
  #warning "Baud rate achieved is lower than allowed"
  #endif
#endif

#ifdef UBRR_VALUE
  #define UBRRL_VALUE (UBRR_VALUE & 0xff)
  #define UBRRH_VALUE (UBRR_VALUE >> 8)
#endif

void usart_init( void ){
	uint8_t temp;

	UCSRA = 0x00;

	/* set baudrate */
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
		UCSRA |= (1<<U2X);
	#else
		UCSRA &= ~(1<<U2X);
	#endif

	/* rx & tx enable */
	UCSRB = (1<<RXEN) | (1<<TXEN);

	/* set frame format */
	// data bits
	temp = 1<<URSEL;
	temp |= ((USART_DATABIT)-5)<<UCSZ0;
	// stop bits
	temp |= ((USART_STOPBIT)-1)<<USBS;
	// parity bit
	temp |= ((USART_PARITYBIT_VALUE(USART_PARITYBIT)))<<UPM0;
	UCSRC = temp;

}

void usart_putc( uint8_t dat ){
	while( !(UCSRA & (1<<UDRE)) )
		;
	UDR = dat;
}

uint8_t usart_getc( void ){
	while( !(UCSRA & (1<<RXC)) )
		;
	return UDR;
}

static int usart_stream_getc( FILE *fd ){
	return usart_getc( );
}

static int usart_stream_putc( char c, FILE *fd ){
	if( c == '\n' )
		usart_putc( '\r');
	usart_putc( c );
	
	return 1;
}

FILE usart_file = FDEV_SETUP_STREAM( 
		usart_stream_putc,
		usart_stream_getc,
		_FDEV_SETUP_RW );

#endif /*  __AVR_ATmega16__, __AVR_ATmega16A__ */

