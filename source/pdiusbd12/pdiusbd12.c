#include <util/delay.h>
#include "pdiusbd12.h"

#ifdef PDIUSBD12_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

/* for A0 */
#define D12_ADDR_SELECT_CMD	1
#define D12_ADDR_SELECT_DATA	0

void d12_cmd( uint8_t cmd ){
	d12_port_DATA_setOutMode();
	d12_pin_CS_setValue( 0 );
	d12_port_DATA_setValue( cmd );
	d12_pin_A0_setValue( D12_ADDR_SELECT_CMD );
	d12_pin_WR_setValue( 0 );
	d12_pin_WR_setValue( 1 );
	d12_pin_CS_setValue( 1 );
}

void d12_init( void ){
	d12_pin_setup();
}

void d12_writeByte( uint8_t data ){
	d12_port_DATA_setOutMode();
	d12_pin_CS_setValue( 0 );
	d12_port_DATA_setValue( data );
	d12_pin_A0_setValue( D12_ADDR_SELECT_DATA );
	d12_pin_WR_setValue( 0 );
	d12_pin_WR_setValue( 1 );
	d12_pin_CS_setValue( 1 );
}

uint8_t d12_readByte( void ){
	uint8_t data;

	d12_port_DATA_setInMode();
	d12_pin_CS_setValue( 0 );
	d12_pin_A0_setValue( D12_ADDR_SELECT_DATA );
	d12_pin_RD_setValue( 0 );
	data = d12_port_DATA_getValue();
	d12_pin_RD_setValue( 1 );
	d12_pin_CS_setValue( 1 );

	return data;
}

/*
 * Command
 */
void d12_connect( void ){
	d12_cmd( SetMode );
	d12_writeByte( 0x16 );
	d12_writeByte( 0x47 );
}

void d12_disconnect( void ){
	d12_cmd( SetMode );
	d12_writeByte( 0x06 );
	d12_writeByte( 0x47 );
	_delay_ms( 1000 );
}

uint16_t d12_getID( void ){
	uint16_t id;
	d12_cmd( ReadID );
	id = d12_readByte();
	id |= d12_readByte()<<8;
	return id;
}

uint8_t d12_getInterrupt( void ){
	uint8_t intr;
	d12_cmd( ReadInterruptRegister );
	intr = d12_readByte();
	return intr;
}

uint8_t d12_getLastTransStatus( uint8_t ep ){
	uint8_t status;
	d12_cmd( ReadLastTransactionStatus(ep) );
	status = d12_readByte();
	return status;
}

uint8_t d12_readBuffer( uint8_t ep, uint8_t *data, uint8_t max_len ){
	uint8_t len;
	uint8_t idx;
	
	PDEBUG_OUTPUT( "read buffer: ");
	d12_selectEndpoint( ep );

	d12_cmd( ReadBuffer );
	d12_readByte();
	len = d12_readByte();

	if( len > max_len )
		len = max_len;

	for( idx=0; idx<len; ++idx ){
		data[idx] = d12_readByte();
		PDEBUG_OUTPUT( "%x ", data[idx] );
	}
	PDEBUG_OUTPUT( "\n" );

	return len;
}

void d12_acknowledgeSetup( void ){
	d12_selectEndpoint( D12_ENDP_CTRL_IN );
	d12_cmd( AcknowledgeSetup );
	d12_selectEndpoint( D12_ENDP_CTRL_OUT );
	d12_cmd( AcknowledgeSetup );
}

void d12_writeBuffer( uint8_t ep, uint8_t *data, uint8_t len ){
	uint8_t idx;

	d12_selectEndpoint( ep );
	d12_cmd( WriteBuffer );
	d12_writeByte( 0 );
	d12_writeByte( len );

	PDEBUG_OUTPUT( "write buff: " );
	for( idx=0; idx<len; ++idx ){
		d12_writeByte( data[idx] );
		PDEBUG_OUTPUT( "%x ", data[idx] );
	}
	PDEBUG_OUTPUT( "\n" );
}

void d12_setAddress( uint8_t addr, uint8_t enable ){
	if( enable )
		enable = 0x80;
	d12_cmd( SetAddressEnable );
	d12_writeByte( enable | addr );
}

void d12_enableEndp( uint8_t enable ){
	d12_cmd( SetEndpointEnable );
	if( enable )
		enable = 0x01;
	d12_writeByte( enable );
}
