#include <stdio.h>
#include <stdarg.h>
#include <util/delay.h>

#include "enc28j60.h"
#include "enc28j60_reg.h"

#define enc28j60_delay_ms(n)	_delay_ms(n)
#define enc28j60_delay_us(n)	_delay_us(n)

unsigned char enc28j60_current_bank;
unsigned int enc28j60_next_packet;

uint8_t __spi_rwByte( uint8_t data ){

	uint8_t idx;
	for( idx=0; idx<8; ++idx ){
		enc28j60_pin_SI_setValue( data & 0x80 );	
		data <<= 1;
		enc28j60_pin_SCK_setValue( 1 );
		if( enc28j60_pin_SO_getValue() )
			data |= 1;
		enc28j60_pin_SCK_setValue( 0 );
	}
	return data;
}


void enc28j60_bitFieldSet( uint8_t reg, uint8_t mask ){
	if( !enc28j60_is_common(reg) )
		enc28j60_selectBank( enc28j60_get_bank(reg) );

	enc28j60_pin_CS_setValue( 0 ); 
	__spi_rwByte( ENC28J60_CMD_BFS |enc28j60_get_addr(reg) );
	__spi_rwByte( mask );
	enc28j60_pin_CS_setValue( 1 );
}
void 	enc28j60_bitFieldClear( uint8_t reg, uint8_t mask ){
	if( !enc28j60_is_common(reg) )
		enc28j60_selectBank( enc28j60_get_bank(reg) );

	enc28j60_pin_CS_setValue( 0 ); 
	__spi_rwByte( ENC28J60_CMD_BFC | enc28j60_get_addr(reg) );
	__spi_rwByte( mask );
	enc28j60_pin_CS_setValue( 1 );	
}

void 	enc28j60_selectBank( uint8_t bank ){
	if( enc28j60_current_bank != bank ){
		enc28j60_current_bank = bank;
		enc28j60_bitFieldClear( ECON1, 0x03 );
		enc28j60_bitFieldSet( ECON1, bank );
	}	
}
uint8_t enc28j60_readCtrlReg( uint8_t reg ){
	uint8_t data;
	
	if( !enc28j60_is_common(reg) )
		enc28j60_selectBank( enc28j60_get_bank(reg) );	

	enc28j60_pin_CS_setValue( 0 );
	__spi_rwByte( ENC28J60_CMD_RCR | enc28j60_get_addr(reg) );
	if( enc28j60_is_miimac( reg ) ){
		__spi_rwByte( 0 );
	}
	data = __spi_rwByte( 0 );
	enc28j60_pin_CS_setValue( 1 );

	return data;
}
void enc28j60_writeCtrlReg( uint8_t reg, uint8_t data ){

	if( !enc28j60_is_common(reg) )
		enc28j60_selectBank( enc28j60_get_bank(reg) );	

	enc28j60_pin_CS_setValue( 0 );
	__spi_rwByte( ENC28J60_CMD_WCR | enc28j60_get_addr(reg) );
	__spi_rwByte( data );
	enc28j60_pin_CS_setValue( 1 );
}

void 	enc28j60_readBufferMem( uint8_t *data, uint8_t len ){
	uint8_t idx;
	
	enc28j60_pin_CS_setValue( 0 );
	__spi_rwByte( ENC28J60_CMD_RBM | 0x1a );
	for( idx=0; idx<len; ++idx ){
		data[idx] = __spi_rwByte( 0 );
	}
	enc28j60_pin_CS_setValue( 1 );
}

void 	enc28j60_writeBufferMem( uint8_t *data, uint8_t len ){
	uint8_t idx;

	enc28j60_pin_CS_setValue( 0 );
	//__spi_rwByte( ENC28J60_CMD_WBM | 0x1a );
	__spi_rwByte( ENC28J60_CMD_WBM );
	for( idx=0; idx<len; ++idx ){
		__spi_rwByte( data[idx] );
	}
	enc28j60_pin_CS_setValue( 1 );
}
void enc28j60_writePhy( uint8_t addr, uint16_t data ){

	enc28j60_writeCtrlReg( MIREGADR, addr );
	enc28j60_writeCtrlReg( MIWRL, data&0xff );
	enc28j60_writeCtrlReg( MIWRH, (data&0xff00)>>8);

	while( enc28j60_readCtrlReg(MISTAT) & MISTAT_BUSY ){
	}
}
uint16_t enc28j60_readPhy( uint8_t addr ){
	uint16_t data;

	enc28j60_writeCtrlReg( MIREGADR, addr );
	enc28j60_bitFieldSet( MICMD, MICMD_MIIRD );

	/* Wait 10.24 us */
	enc28j60_delay_us( 11 );
	while( enc28j60_readCtrlReg(MISTAT) & MISTAT_BUSY ){
	}
	enc28j60_bitFieldClear( MICMD, MICMD_MIIRD );


	data = enc28j60_readCtrlReg( MIRDH ) <<8;
	data += enc28j60_readCtrlReg( MIRDL );

	return data;
}
void	enc28j60_softReset( void ){
	enc28j60_pin_CS_setValue( 0 ); 
	__spi_rwByte( ENC28J60_CMD_SC | 0x1f );
	enc28j60_pin_CS_setValue( 1 );
	enc28j60_delay_ms( 50 );
}

void enc28j60_hardReset( void ){
	enc28j60_pin_RESET_setVakue( 0 );
	enc28j60_delay_ms( 10 );
	enc28j60_pin_RESET_setVakue( 1 );
	enc28j60_delay_ms( 50 );
}
void enc28j60_transmitReset( void );
void enc28j60_receiveReset( void );
void enc28j60_phyReset( void );
void enc28j60_macReset( void );

void enc28j60_init( uint8_t *mac_addr ){

	enc28j60_current_bank = 0;
	enc28j60_next_packet = ENC28J60_CFG_RX_BUFF_START;
	enc28j60_pin_setup();

	/* System reset */
	enc28j60_softReset();
	enc28j60_delay_ms( 50 );

	while( !( enc28j60_readCtrlReg(ESTAT) & ESTAT_CLKRDY ) );
	/* 1. Receive buffer */
	enc28j60_writeCtrlReg( ERXSTL, ENC28J60_CFG_RX_BUFF_START&0xff );
	enc28j60_writeCtrlReg( ERXSTH, ENC28J60_CFG_RX_BUFF_START>>8 );

	enc28j60_writeCtrlReg( ERXRDPTL, ENC28J60_CFG_RX_BUFF_START&0xff );
	enc28j60_writeCtrlReg( ERXRDPTH, ENC28J60_CFG_RX_BUFF_START>>8 );

	enc28j60_writeCtrlReg( ERXNDL, ENC28J60_CFG_RX_BUFF_STOP&0xff );
	enc28j60_writeCtrlReg( ERXNDH, ENC28J60_CFG_RX_BUFF_STOP>>8 );

	enc28j60_writeCtrlReg( ETXSTL, ENC28J60_CFG_TX_BUFF_START&0xff );
	enc28j60_writeCtrlReg( ETXSTH, ENC28J60_CFG_TX_BUFF_START>>8 );

	/* 2. Transmit buffer
	 * A memory which is not used by the receive
	 * buffer is considered the transmission buffer
	 */
	/* TODO */
	
	/* 3. Receive filters */
	/*
	enc28j60_recvFilterConfig(
			ENC28J60_FILTER_AND | 
			ENC28J60_FILTER_UNICAST |
			ENC28J60_FILTER_CRC );
	*/
	/* TODO filters:pm */
	
	/* 4. Wait for OSC
	 * If the initialization procedure is being 
	 * executed immediately following a Power-on 
	 * Reset, the ESTAT.CLKRDY bit should be 
	 * polled to make certain that enough time 
	 * has elapsed before proceeding to modify 
	 * the MAC and PHY registers.
	 */
	while( !(enc28j60_readCtrlReg( ESTAT ) & ESTAT_CLKRDY ) );
	
	/* 5. MAC Initialization setting */
	/* 5.1 Clear the MARST bit in MACON2 to pull
	 * the MAC out of Reset. */
	/* 
	enc28j60_bitFieldClear( ENC28J60_REG_MACON2,
			ENC28J60_BIT_MARST );
	*/
	enc28j60_writeCtrlReg( MACON2, 0x00 );

	/* 5.2 Set the MARXEN bit in MACON1 to enable
	 * the MAC to receive frames. If using full
	 * duplex, most applications should also set 
	 * TXPAUS and RXPAUS to allow IEEE defined
	 * flow control to function */
	enc28j60_bitFieldSet( MACON1,
			MACON1_MARXEN |
			MACON1_TXPAUS |
			MACON1_RXPAUS );
	
	/* 5.3 Configure the PADCFG, TXCRCEN and 
	 * FULDPX bits of MACON3. Most applications
	 * should enable automatic padding to at
	 * least 60 bytes and always append a valid
	 * CRC. For convenience, many applications 
	 * may wish to set the FRMLNEN bit as well
	 * to enable frame length status reporting. 
	 * The FULDPX bit should be set if the 
	 * application will be connected to a 
	 * full-duplex configured remote node; 
	 * otherwise, it should be left clear.
	 */
	enc28j60_bitFieldSet( MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN );
	/* 5.4 Configure the bits in MACON4. Many 
	 * applications may not need to modify the 
	 * Reset default */
			 
	/* 5.5 Program the MAMXFL registers with the 
	 * maximum frame length to be permitted to 
	 * be received or transmitted. Normal network 
	 * nodes are ldesigned to handle packets 
	 * that are 1518 byte or less. */
	enc28j60_writeCtrlReg( MAMXFLL, ENC28J60_CFG_MAX_FRAME_LEN & 0xff );
	enc28j60_writeCtrlReg( MAMXFLH, ENC28J60_CFG_MAX_FRAME_LEN >> 8 );

	/* 5.6 Configure the Back-to-Back Inter-Packet 
	 * Gap register, MABBIPG. Most applications 
	 * will program this register with 15h when 
	 * Full-Duplex mode is used and 12h when 
	 * Half-Duplex mode is used */
	enc28j60_writeCtrlReg( MABBIPG, 0x12 );


	/*  5.7 Configure the Non-Back-to-Back Inter-Packet 
	 *  Gap register low byte, MAIPGL. Most 
	 *  applications will program this register 
	 *  with 12h */
	enc28j60_writeCtrlReg( MAIPGL, 0x12 );

	/* 5.8 If half duplex is used, the Non-Back-to-Back 
	 * Inter-Packet Gap register high byte, MAIPGH, 
	 * should be programmed. Most applications will 
	 * program this register to 0Ch. */
	enc28j60_writeCtrlReg( MAIPGH, 0x0c );
	
	/* 5.9 If Half-Duplex mode is used, program the 
	 * Retransmission and Collision Window registers, 
	 * MACLCON1 and MACLCON2. Most applications 
	 * will not need to change the default Reset values.
	 * If the network is spread over exceptionally long
	 * cables, the default value of MACLCON2 may need 
	 * to be increased. */
	
	/* 5.10 Program the local MAC address into the
	 * MAADR0:MAADR5 registers */
	enc28j60_writeCtrlReg( MAADR0, mac_addr[0] );
	enc28j60_writeCtrlReg( MAADR1, mac_addr[1] );
	enc28j60_writeCtrlReg( MAADR2, mac_addr[2] );
	enc28j60_writeCtrlReg( MAADR3, mac_addr[3] );
	enc28j60_writeCtrlReg( MAADR4, mac_addr[4] );
	enc28j60_writeCtrlReg( MAADR5, mac_addr[5] );

	/* Initizalize PHY */
	enc28j60_writePhy( PHCON2,PHCON2_HDLDIS );

	enc28j60_bitFieldSet( EIE, EIE_INTIE | EIE_PKTIE );

	enc28j60_bitFieldSet( ECON1, ECON1_RXEN );
}

/* ****************************** */
// Transmit Data
/* ****************************** */

void enc28j60_transmitPacket( uint8_t *data, uint16_t len, uint8_t new_conf ){

	/* Write Tx Buffer start address*/
	enc28j60_writeCtrlReg( ETXSTL, ENC28J60_CFG_TX_BUFF_START&0xff );
	enc28j60_writeCtrlReg( ETXSTH, ENC28J60_CFG_TX_BUFF_START>>8 );

	/* TODO This is necessary ? */
	enc28j60_writeCtrlReg( EWRPTL, ENC28J60_CFG_TX_BUFF_START&0xff );
	enc28j60_writeCtrlReg( EWRPTH, ENC28J60_CFG_TX_BUFF_START>>8 );
	/*
	printf( "current buff ptr: %u\n",
			enc28j60_readCtrlReg(EWRPTL) +
			enc28j60_readCtrlReg(EWRPTH)<<8 );
	*/

	/* Write buffer */
	enc28j60_writeBufferMem( &new_conf, 1 );
	enc28j60_writeBufferMem( data, len );

	/* Write Tx buffer end address */
	enc28j60_writeCtrlReg( ETXNDL, (ENC28J60_CFG_TX_BUFF_START+len)&0xff );
	enc28j60_writeCtrlReg( ETXNDH, (ENC28J60_CFG_TX_BUFF_START+len)>>8 );

	/* start transmiting */
	enc28j60_bitFieldSet( ECON1, ECON1_TXRTS );
	
	/* wait.. */
	while( enc28j60_readCtrlReg(ECON1) & ECON1_TXRTS );
}


uint16_t enc28j60_receivePacket( uint8_t *data, uint16_t len ){
	uint16_t pack_len;
	uint16_t pack_status;

	/* TODO check if a packet has been received */
	if( !( enc28j60_readCtrlReg( EIR) & EIR_PKTIF ) ){
		return 0;
	}

	/* set the read pointer */
	enc28j60_writeCtrlReg( ERDPTL, enc28j60_next_packet&0xff );
	enc28j60_writeCtrlReg( ERDPTH, enc28j60_next_packet>>8 );

	/* the next packet pointer */
	enc28j60_readBufferMem( (uint8_t*)&enc28j60_next_packet, 2 );
	/* the packet length */
	enc28j60_readBufferMem( (uint8_t*)&pack_len, 2 );

	/* the status */
	enc28j60_readBufferMem( (uint8_t*)&pack_status, 2 );

	if( pack_len > len ) pack_len = len;

	enc28j60_readBufferMem( data, pack_len );

	/* Freeing receive buffer space */
	enc28j60_writeCtrlReg( ERXRDPTL, enc28j60_next_packet&0xff );
	enc28j60_writeCtrlReg( ERXRDPTH, enc28j60_next_packet>>8 );

	/* Decrement the packet counter */
	enc28j60_bitFieldSet( ECON2, ECON2_PKTDEC );
	
	return pack_len;
}
