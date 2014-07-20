#ifndef _H_PDIUSBD12_
#define _H_PDIUSBD12_

#include <stdint.h>

/*
 * Configurations
 */
#define D12_BUFFER_SIZE		16

/* Endpoint */
#define D12_ENDP_CTRL_OUT	0
#define D12_ENDP_CTRL_IN	1
#define D12_ENDP_1_OUT		2
#define D12_ENDP_1_IN		3
#define D12_ENDP_2_OUT		4
#define D12_ENDP_2_IN		5

/* Size of endpoint buffer */
#define D12_BUFF_SEIZE_CTRL_IN	16

/********************************************************
 * Command
 ********************************************************/
/* Initialize Commands */
#define SetAddressEnable		0xD0 //Write 1 byte
#define SetEndpointEnable		0xD8 //Write 1 byte
#define SetMode				0xF3 //Write 2 bytes
#define SetDMA				0xFB //Write/Read 1 byte

/* Data Flow Commands */
#define ReadInterruptRegister		0xF4 //Read 2 bytes
#define SelectEndpoint(ep)		(ep)
	/*
	Control_OUT 	00 Read 1 byte (optional)
	Control_IN 	01 Read 1 byte (optional)
	Endpoint1_OUT 	02 Read 1 byte (optional)
	Endpoint1_IN 	03 Read 1 byte (optional)
	Endpoint2_OUT 	04 Read 1 byte (optional)
	Endpoint2_IN 	05 Read 1 byte (optional)
	*/

#define ReadLastTransactionStatus(ep)	(0x40+ep)
	/* 
	Control OUT 40 Read 1 byte
	Control_IN 41 Read 1 byte
	Endpoint1_OUT 42 Read 1 byte
	Endpoint1_IN 43 Read 1 byte
	Endpoint2_OUT 44 Read 1 byte
	Endpoint2_IN 45 Read 1 byte
	*/

#define ReadBuffer			0xF0 //Read n bytes
#define WriteBuffer			0xF0 //Write n bytes
#define SetEndpointStatus(ep) 		(0x40+ep)
	/* 
	Control_OUT 	40 Write 1 byte
	Control_IN 	41 Write 1 byte
	Endpoint1_OUT 	42 Write 1 byte
	Endpoint1_IN 	43 Write 1 byte
	Endpoint2_OUT 	44 Write 1 byte
	Endpoint2_IN 	45 Write 1 byte
	*/

#define AcknowledgeSetup 		0xF1 
#define ClearBuffer			0xF2
#define ValidateBuffer 			0xFA

/* General commands */
#define SendResume 			0xF6
#define ReadCurrentFrameNumber 		0xF5 //Read 1 or 2 bytes
#define ReadID				0xFD

/*
 * Interrupt Bit
 */
#define INTR_CTRL_OUT			0x01
#define INTR_CTRL_IN			0x02
#define INTR_ENP1_OUT			0x04
#define INTR_ENP1_IN			0x08
#define INTR_MAIN_OUT			0x10
#define INTR_MAIN_IN			0x20
#define INTR_BUS_RESET			0x40
#define INTR_SUSPEND_CHANGE		0x80

/* The size of buffer */
#define D12_BUFF_SIZE_CTRL_IN		16


/*
 * Low-level operations
 */
void d12_init( void );
void d12_cmd( uint8_t cmd );
void d12_writeByte( uint8_t data );
uint8_t d12_readByte( void );

/*
 * Command level
 */
#define d12_selectEndpoint(ep)	d12_cmd(SelectEndpoint(ep))
#define d12_clearBuffer()	d12_cmd(ClearBuffer)
#define d12_validateBuffer()	d12_cmd(ValidateBuffer)

void     d12_connect( void );
void     d12_disconnect( void );
uint16_t d12_getID( void );
uint8_t  d12_getInterrupt( void );
uint8_t  d12_getLastTransStatus( uint8_t ep );
uint8_t  d12_readBuffer( uint8_t ep, uint8_t *data, uint8_t max_len );
void     d12_acknowledgeSetup( void );
void     d12_writeBuffer( uint8_t ep, uint8_t *data, uint8_t len );
void     d12_setAddress( uint8_t addr, uint8_t enable );
void     d12_enableEndp( uint8_t enable );

/* 
 * Pins operations 
 */
void	d12_pin_setup( void );
void	d12_pin_CS_setValue( uint8_t v );
void	d12_pin_A0_setValue( uint8_t v );
void	d12_pin_WR_setValue( uint8_t v );
void	d12_pin_RD_setValue( uint8_t v );
void	d12_port_DATA_setValue( uint8_t v );
void	d12_port_DATA_setOutMode( void );
void	d12_port_DATA_setInMode( void );
uint8_t d12_port_DATA_getValue( void );
uint8_t d12_pin_INT_getValue( void );


#endif
