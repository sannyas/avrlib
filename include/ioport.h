/**
 * @fil	   ioport.h
 * @brief  The definition of pins and some operations.
 * @author Liao Maoyi
 * @date:  2013/10/09
 *
 * The avr-libc library has defined some macro of pins 
 * to make pin operations easier. PA0 to PA7 are defined
 * as 0 to 7. The PB,PC,PD are the same. In order to not
 * conflict with already defined macro, in this file 
 * every pins are defined as PORT[ABCD]_x( x is 0 to 7 )
 * and every ports are defined as PORT[ABCD]. other
 * multiplexed pins also use a macro defination to represent
 * the associated pin.
 *
 * Usage:
 *   + Set input or output mode.
 *       iopin_outmode( PORT_[ABCD]x );
 *       iopin_inmode( PORT_[ABCD]x );
 *       ioport_outmode( PORT_[ABCD] );
 *       ioport_inmode( PORT_[ABCD] );
 *   + Output
 *       iopin_outhigh( PORT_[ABCD]x );
 *       iopin_outlow( PORT_[ABCD]x );
 *       iopin_out( PORT_[ABCD]x, bool );
 *       The ioport is the same.
 *   + Input
 *       iopin_get( PORT_[ABCD]x );
 *       ioport_get( PORT_[ABCD] );
 *
 * Medified:
 * 	Data: 2014/04/03
 * 	Add interrupt pins for atmega16
 *
 */

#ifndef _IOCTRL_HEADER_
#define _IOCTRL_HEADER_

#include <avr/io.h>

/* ****************************** */
// For ATmega16
/* ****************************** */
#if defined( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

/* Extern interrup */
#define PORT_INT0	PORT_D2
#define PORT_INT1	PORT_D3
#define PORT_INT2	PORT_D4

/* SPI */
#define PORT_SS		PORT_B4
#define PORT_MOSI	PORT_B5
#define PORT_MISO	PORT_B6
#define PORT_SCK	PORT_B7

/* ADC */
#define PORT_ADC0	PORT_A0
#define PORT_ADC1	PORT_A1
#define PORT_ADC2	PORT_A2
#define PORT_ADC3	PORT_A3
#define PORT_ADC4	PORT_A4
#define PORT_ADC5	PORT_A5
#define PORT_ADC6	PORT_A6
#define PORT_ADC7	PORT_A7


/* GPIO */
#define PORT_A	A
#define PORT_B	B
#define PORT_C	C
#define PORT_D	D

#define PORT_A0	A0
#define PORT_A1	A1
#define PORT_A2	A2
#define PORT_A3	A3
#define PORT_A4	A4
#define PORT_A5	A5
#define PORT_A6	A6
#define PORT_A7	A7

#define PORT_B0	B0
#define PORT_B1	B1
#define PORT_B2	B2
#define PORT_B3	B3
#define PORT_B4	B4
#define PORT_B5	B5
#define PORT_B6	B6
#define PORT_B7	B7

#define PORT_C0	C0
#define PORT_C1	C1
#define PORT_C2	C2
#define PORT_C3	C3
#define PORT_C4	C4
#define PORT_C5	C5
#define PORT_C6	C6
#define PORT_C7	C7

#define PORT_D0	D0
#define PORT_D1	D1
#define PORT_D2	D2
#define PORT_D3	D3
#define PORT_D4	D4
#define PORT_D5	D5
#define PORT_D6	D6
#define PORT_D7	D7

#define PA0_DDR	DDRA
#define PA1_DDR	DDRA
#define PA2_DDR	DDRA
#define PA3_DDR	DDRA
#define PA4_DDR	DDRA
#define PA5_DDR	DDRA
#define PA6_DDR	DDRA
#define PA7_DDR	DDRA

#define PA0_PORT PORTA
#define PA1_PORT PORTA
#define PA2_PORT PORTA
#define PA3_PORT PORTA
#define PA4_PORT PORTA
#define PA5_PORT PORTA
#define PA6_PORT PORTA
#define PA7_PORT PORTA

#define PA0_PIN	PINA
#define PA1_PIN	PINA
#define PA2_PIN	PINA
#define PA3_PIN	PINA
#define PA4_PIN	PINA
#define PA5_PIN	PINA
#define PA6_PIN	PINA
#define PA7_PIN	PINA


#define PB0_DDR	DDRB
#define PB1_DDR	DDRB
#define PB2_DDR	DDRB
#define PB3_DDR	DDRB
#define PB4_DDR	DDRB
#define PB5_DDR	DDRB
#define PB6_DDR	DDRB
#define PB7_DDR	DDRB

#define PB0_PORT PORTB
#define PB1_PORT PORTB
#define PB2_PORT PORTB
#define PB3_PORT PORTB
#define PB4_PORT PORTB
#define PB5_PORT PORTB
#define PB6_PORT PORTB
#define PB7_PORT PORTB

#define PB0_PIN	PINB
#define PB1_PIN	PINB
#define PB2_PIN	PINB
#define PB3_PIN	PINB
#define PB4_PIN	PINB
#define PB5_PIN	PINB
#define PB6_PIN	PINB
#define PB7_PIN	PINB


#define PC0_DDR	DDRC
#define PC1_DDR	DDRC
#define PC2_DDR	DDRC
#define PC3_DDR	DDRC
#define PC4_DDR	DDRC
#define PC5_DDR	DDRC
#define PC6_DDR	DDRC
#define PC7_DDR	DDRC

#define PC0_PORT PORTC
#define PC1_PORT PORTC
#define PC2_PORT PORTC
#define PC3_PORT PORTC
#define PC4_PORT PORTC
#define PC5_PORT PORTC
#define PC6_PORT PORTC
#define PC7_PORT PORTC

#define PC0_PIN	PINC
#define PC1_PIN	PINC
#define PC2_PIN	PINC
#define PC3_PIN	PINC
#define PC4_PIN	PINC
#define PC5_PIN	PINC
#define PC6_PIN	PINC
#define PC7_PIN	PINC


#define PD0_DDR	DDRD
#define PD1_DDR	DDRD
#define PD2_DDR	DDRD
#define PD3_DDR	DDRD
#define PD4_DDR	DDRD
#define PD5_DDR	DDRD
#define PD6_DDR	DDRD
#define PD7_DDR	DDRD

#define PD0_PORT PORTD
#define PD1_PORT PORTD
#define PD2_PORT PORTD
#define PD3_PORT PORTD
#define PD4_PORT PORTD
#define PD5_PORT PORTD
#define PD6_PORT PORTD
#define PD7_PORT PORTD

#define PD0_PIN	PIND
#define PD1_PIN	PIND
#define PD2_PIN	PIND
#define PD3_PIN	PIND
#define PD4_PIN	PIND
#define PD5_PIN	PIND
#define PD6_PIN	PIND
#define PD7_PIN	PIND

#else 
  #warning Device is not supported.
#endif 


#define _iopin_outmode( Xn )\
		(P##Xn##_DDR |= _BV(P##Xn))
#define _iopin_inmode( Xn )\
		(P##Xn##_DDR &= ~_BV(P##Xn))
#define _iopin_sethigh( Xn )\
		(P##Xn##_PORT |= _BV(P##Xn))
#define _iopin_setlow( Xn )\
		(P##Xn##_PORT &= ~_BV(P##Xn))
#define _iopin_get( Xn )\
		((P##Xn##_PIN & (1<<P##Xn))>>P##Xn )

#define _ioport_outmode( Px )\
		( DDR##Px = 0xff )
#define _ioport_inmode( Px )\
		( DDR##Px = 0x00 )
#define _ioport_set( Px, v )\
		( PORT##Px = v )
#define _ioport_get( Px )\
		( PIN##Px )

#define iopin_outmode( Xn )	_iopin_outmode( Xn )
#define iopin_inmode( Xn ) 	_iopin_inmode( Xn )
#define iopin_outhigh( Xn )	_iopin_sethigh( Xn )
#define iopin_outlow( Xn )	_iopin_setlow( Xn )
#define iopin_out( Xn, v )	((v)? _iopin_sethigh(Xn) : _iopin_setlow(Xn) )
#define iopin_pullup( Xn )	_iopin_sethigh( Xn )
#define iopin_nopull( Xn )	_iopin_setlow( Xn )
#define iopin_get( Xn )		_iopin_get(Xn)

#define ioport_outmode( Px )	_ioport_outmode( Px )
#define ioport_inmode( Px )	_ioport_inmode( Px )
#define ioport_out( Px, v )	_ioport_set( Px, v )
#define ioport_get( Px )	_ioport_get( Px )
#define ioport_pullup( Px )	_ioport_set( Px, 0xff )
#define ioport_nopull( Px )	_ioport_set( Px, 0x00 )


#endif

