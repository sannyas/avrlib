#ifndef _ST7920_HEADER_
#define _ST7920_HEADER_

#include <stdint.h>

/*
 * module name	: ST7920
 * note		: api for chip st7920
 * 		  There is two data transfer mode can be selected
 * 		  	1. 8-bit parallel bus mode
 * 		  	2. serial mode
 * author	: MY liao
 * data time	: 2013/09/17
 * log		:
 * 		2013/09/17	8-bit Parallel Bus Mode Data Transfer
 * 		2013/09/18	serial communication
 */

/*
 * PIN:
 * 1. GND	ground
 * 2. VCC	
 * 3. V0	Input of LCD driving voltage
 * 4. RS(CS)	Parallel command/data selection signal( serial chip select )
 * 5. R/W(SID)	Parallel raad/write selection signal( serial data port ) 
 * 6. E(CLK)	Parallel enable signal( serial synchronizing signal )
 * 7. BD0-7	Data bus
 * 8. PSB	Parallel/serial selection
 * 9. /RST	Reset
 * 10. A	LED +5	
 * 11. K	LED 0
 */


/* configuration avaliabled 
 *  Data Bus:
 *    ST7920_PARALLEL_8
 *      ST7920_DB4(dat)	xxx
 *    ST7920_PARALLEL_4
 *      ST7920_DB8(dat)	xxx
 *    ST7920_SERIAL
 *  MCU type:
 *    MCU_801
 *    MCU_AVR
 *  MCU specific:
 */

/* Example of configure for AVR */
 #define MCU_AVR		1
/* -- 8-bit Parallel */
 #define ST7920_PARALLEL_8	1

 #define ST7920_PIN_DB		PORT_A
 #define ST7920_PIN_RS		PORT_B0
 #define ST7920_PIN_RW		PORT_B1
 #define ST7920_PIN_E		PORT_B2
 #define ST7920_PIN_RESET	PORT_B3

/* -- serial */
/*
 #define ST7920_SERIAL		1

 #define ST7920_PIN_CS		PORT_A0
 #define ST7920_PIN_SID		PORT_A1
 #define ST7920_PIN_SCLK	P0RT_A2
*/

/* Configure end */


/* Configure handle for AVR */
#if defined( MCU_AVR )
 #include <avr/io.h>
 #include "hard_cfg.h"
 #include <util/delay.h>
 #include <ioport.h>

 #define st7920_delay_ms(v)	_delay_ms(v)
 #define st7920_delay_us(v)	_delay_us(v)

 #if defined( ST7920_PARALLEL_8 )
  #define ST7920_DB(v)	ioport_out( ST7920_PIN_DB,v )
  #define ST7920_DB_GET(v)\
			ioport_inmode( ST7920_PIN_DB );\
			ioport_nopull( ST7920_PIN_DB );\
			st7920_delay_ms( 1 );\
			v = ioport_get( ST7920_PIN_DB );\
			ioport_outmode( ST7920_PIN_DB )
  #define ST7920_RS(v)	(iopin_out(ST7920_PIN_RS,v))
  #define ST7920_RW(v)	(iopin_out(ST7920_PIN_RW,v))
  #define ST7920_E(v)	(iopin_out(ST7920_PIN_E ,v))
  #define ST7920_RESET(v)\
			(iopin_out(ST7920_PIN_RESET,v))
 #endif

 #if defined( ST7920_SERIAL )
  #define ST7920_CS(v)	iopin_out( ST7920_PIN_CS, v )
  #define ST7920_SCLK(v)\
			iopin_out( ST7920_PIN_SCLK,v)
  #define ST7920_SID(v)	iopin_out( ST7920_PIN_SID, v)
  #define ST7920_SID_GET(v)\
			iopin_inmode( ST7920_PIN_SID );\
			iopin_nopull( ST7920_PIN_SID );\
			v = iopin_get( ST7920_PIN_SID );\
			iopin_outmode( ST7920_PIN_SID )
 #endif

#endif

/* END -- Configure handle for AVR */


/*-------- command define ------- */

/* basic command */
#define   CMD_CLEAN		0x01
#define   CMD_AC_RESET		0x02
#define   CMD_ENTRY_MODE	0x04
  #define CMD_ENTRY_AC_INC	0x02
  #define CMD_ENTRY_ALL_DIS	0x01
#define   CMD_DIS_STATUS	0x08
  #define CMD_ALL_DISPLAY	0x04
  #define CMD_CURSOR_SHOW	0x02
  #define CMD_CURSOR_INV	0x01
#define   CMD_CURSOR_MOVE	0x10
  #define CMD_CURSOR_RIGHT	0x04
  #define CMD_DISPLAY_MOVE	0x08
#define   CMD_FUNCTION		0x20
  #define CMD_8BITS		0x10
  #define CMD_EXT		0x04
#define   CMD_CGRAM_AC		0x40
#define   CMD_DDRAM_AC		0x80

/* extern command */
#define   ECMD_WAIT_ORDER	0x01
#define   ECMD_SCROLL_IRAM	0x02
  #define ECMD_SCROLL		0x01
  #define ECMD_IRAM		0x00
#define	  ECMD_INVERSE		0x04
#define   ECMD_SLEEP		0x08
  #define ECMD_WAKEUP		0x09
#define   ECMD_EXT_FUNC		0x20
  #define ECMD_8B		0x10
  #define ECMD_EXT		0x04
  #define ECMD_GTIP		0x02
#define   ECMD_SCROLL_IRAM_AC	0x40
#define   ECMD_GDRAM_AC		0x80
  #define ECMD_GDRAM_AC_H	0x80
  #define ECMD_GDRAM_AC_V	0x80


#if	defined( ST7920_PARALLEL_8 )
#define st7920_init( )		st7920_p8_init  ( )
#define st7920_cmd(c)		st7920_p8_cmd   (c)
#define st7920_status( )	st7920_p8_status( )
#define st7920_read( )		st7920_p8_read  ( )
#define st7920_write(d)		st7920_p8_write (d)

#elif	defined( ST7920_SERIAL )
#define st7920_init( )		st7920_s_init  ( )
#define st7920_cmd(c)		st7920_s_cmd   (c)
#define st7920_status( )	st7920_s_status( )
#define st7920_read( )		st7920_s_read  ( )
#define st7920_write(d)		st7920_s_write (d)

#endif

/*-------- 8-bit parallel --------*/

#ifdef ST7920_PARALLEL_8
 
void	st7920_p8_init  ( void 	      );
void	st7920_p8_cmd   ( uint8_t c   );
uint8_t	st7920_p8_status( void        );
void	st7920_p8_write ( uint8_t dat );
uint8_t	st7920_p8_read  ( void        );

#endif /* ST7920_PARALLEL_8*/

/*----------- serial ------------*/

#ifdef ST7920_SERIAL

/*
#define ST7920_CS	P0_0
#define ST7920_SID	P0_1
#define ST7920_SCLK	P0_2
*/
			
#define 	ST7920_MOD_CMD		0
#define		ST7920_MOD_STATUS	1
#define 	ST7920_MOD_READ		2
#define 	ST7920_MOD_WRITE	3

void	st7920_s_byte_write( uint8_t dat, uint8_t mode );
uint8_t	st7920_s_byte_read( uint8_t mode );

void	st7920_s_init( void );
#define	st7920_s_cmd(c)		st7920_s_byte_write( c, ST7920_MOD_CMD )
#define	st7920_s_status( )	st7920_s_byte_read( ST7920_MOD_STATUS )
#define	st7920_s_write(dat)	st7920_s_byte_write( dat, ST7920_MOD_WRITE )
#define	st7920_s_read( )	st7920_s_byte_read( ST7920_MOD_READ );

#endif /* ST7920_SERIAL */


/* General operation */
#define st7920_isBusy( )	( st7920_status() & 1<<7 )
void st7920_clear( );
void st7920_nl( );
void st7920_cr( );
void st7920_backspace( );
void st7920_scollup( ); /* !! */
void st7920_putc( uint8_t dat );


/* stdio for AVR */
#if defined( MCU_AVR )
#include <stdio.h>
extern FILE st7920_file;
#endif

#endif
