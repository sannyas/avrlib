#ifndef _H_SPI_
#define _H_SPI_

#include <avr/io.h>

#if defined( __AVR_ATmega16A__ ) \
	|| defined( __AVR_ATmega16__ )

#define SPI_INTERRUP_ENABLE	_BV(SPIE)
#define SPI_INTERRUP_DISABLE	0x00
#define SPI_ENABLE		_BV(SPE)
#define SPI_DISABLE		0x00
#define SPI_FIRST_LSB		_BV(DORD)
#define SPI_FIRST_MSB		0x00
#define SPI_MASTER		_BV(MSTR)
#define SPI_SLAVE		0x00
#define SPI_IDLE_HIGH		_BV(CPOL)
#define SPI_IDLE_LOW		0x00
#define SPI_SAMPLING_TRAILLING	_BV(CPHA)
#define SPI_SAMPLING_LEADING	0x00
#define SPI_FREQUENCY_DIV4	0x00
#define SPI_FREQUENCY_DIV16	0x01
#define SPI_FREQUENCY_DIV64	0x02
#define SPI_FREQUENCY_DIV128	0x03
#define SPI_CFG_DEFAULT		(SPI_INTERRUP_DISABLE \
				| SPI_DISABLE \
				| SPI_LSB_FIRST \
				| SPI_IDLE_LOW \
				| SPI_SAMPLING_TRAILLING \
				| SPI_FREQUENCY_DIV16 )



/**
 * @brief SPI master config
 *
 * @param cfg [in] SPI_INTERRUP_* SPI_FIRST_* | SPI_IDLE_* | SPI_SAMPLING_* \
 * 			| SPI_FREQUENCY_DIV*
 */
extern inline spi_masterConfig( uint8_t cfg ){
	ioport_outmode( PORT_MOSI );
	ioport_outmode( PORT_SCK );
	/* TODO PORT_SS */

	SPCR = SPI_MASTER | cfg;
}

extern inline spi_masterInit( void ){
	ioport_outmode( PORT_MOSI );
	ioport_outmode( PORT_SCK );
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

extern inline spi_enable( void ){
	SPCR |= _BV(SPE);
}

extern inline spi_disable( void ){
	SPCR &= ~_BV(SPE);
}

extern inline spi_masterTarnsmit( uint8_t data ){
	SPDR = data;
	/* waiting for completed */
	while(!(SPSR & (1<<SPIF)));
	/* TODO interrupt */
}

extern inline spi_masterReceive( void ){
	return SPDR;
}

#endif /* __AVR_ATmega16__ */

#endif
