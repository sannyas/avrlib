/**
 * @file usart.h
 *
 * The detail description \ref grp_avr "USART".
 */

/**
 * @addtogroup grp_avr AVR
 * @{
 */

/**
 * @addtogroup grp_usart USART

 * @brief USART module on the AVR chip 
 *
 * This file implement some basic operation for USART. 
 * Before using USART, some macro are need to define.But 
 * this implemetation don't support dynamic configuration.
 * So this module must be configured at compile time in
 * Makefile.
 * <pre>
 * These configurations include :
 *
 *   - USART_BAUD	9600|...
 *   - USART_DATABIT	5|6|7|8
 *   - USART_STOPBIT	1|2
 *   - USART_PARITYBIT	EVEN|ODD|NONE
 *
 * Sample code without stdio:
 * @code
 * #include <stdio.h>
 * #include <usart.h>
 * void usart_sample( void ){
 *     uint8_t data;
 *     usart_init();
 *     usart_putc( "o" );
 *     usart_putc( "k" );
 *     data = usart_getc();
 * }
 * @endcode
 * 	
 * Sample code using stdio:
 * @code
 * #include <stdio.h>
 * #include <usart.h>
 * void usart_sample( void ){
 *     usart_init();
 *     stdio = &usart_file;
 *     printf( "hello, world\n" );
 * }
 * @endcode
 * </pre>
 * @{
 */

#ifndef _USART_HDR_
#define _USART_HDR_
#include <stdint.h>
#include <stdio.h>

#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

/**
 * @brief The input and output stream used by avrlibc/stdio.
 */
extern FILE usart_file;

/**
 * @brief Initialize usart.
 */
void	usart_init( void );

/**
 * @brief Output a charactor.
 */
void	usart_putc( uint8_t dat );

/**
 * @brief Get a charactor.
 */
uint8_t	usart_getc( void );

#endif /*  __AVR_ATmega16__, __AVR_ATmega16A__ */

#endif

/** @} */
/** @} */
