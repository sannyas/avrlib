#include <stdio.h>

#ifdef NRF24L01P_DEBUG
  #define AVRLIB_DEBUG
#endif
#ifdef AVRLIB
  #include <debug.h>
#else
  #define DEBUG_OUTPUT(fmt,args...)	printf( fmt,##args );
#endif

#include "nrf24l01p.h"


/****************************************************/
/*	Command
 ****************************************************/

/**
 * @brief SPI Commands.
 *
 * Every new commands must be started by a high to low transition
 * on CSN. The serial shifting SPI commands is in the following 
 * format:
 * 	<Command word>MSBit to LSBit; <Data bytes>LSByte to MSByte.
 * @{
 */
#define R_REGISTER(reg)		(reg)		// 5-bit addr
#define W_REGISTER(reg)		(0x20|(reg))	// 5-bit addr
#define R_RX_PAYLOAD		(0x61)
#define W_TX_PAYLOAD		(0xa0)
#define FLUSH_TX		(0xe1)
#define FLUSH_RX		(0xe2)
#define REUSE_TX_PL		(0xe3)
#define R_RX_PL_WID		(0x60)
#define W_ACK_PAYLOAD(pipe)	(0xa8|(pipe))
#define W_TX_PAYLOAD_NO_ACK	(0xb0)
#define NOP			(0xff)
/*@}*/


/*******************************************************/
/*	Registers
 *******************************************************/
/**
 * @brief Register map table.
 * @{
 */
#define CONFIG			0x00
#define EN_AA			0x01 // Enable auto ack.
#define EN_RXADDR		0x02 // Enable RX data pipe.
#define SETUP_AW		0x03 // Set addr width.
#define SETUP_RETR		0x04 // Setup of auto retransmission.
#define RF_CH			0x05 // RF channel.
#define RF_SETUP		0x06 // RF setup reg.
#define STATUS			0x07 // Status reg.
#define OBSERVE_TX		0x08
#define RPD			0x09 // Received power detector.
#define RX_ADDR_P0		0x0a 
#define RX_ADDR_P1		0x0b
#define RX_ADDR_P2		0x0c
#define RX_ADDR_P3		0x0d
#define RX_ADDR_P4		0x0e
#define RX_ADDR_P5		0x0f
#define TX_ADDR			0x10
#define RX_PW_P0		0x11
#define RX_PW_P1		0x12
#define RX_PW_P2		0x13
#define RX_PW_P3		0x14
#define RX_PW_P4		0x15
#define RX_PW_P5		0x16
#define FIFO_STATUS		0x17
#define DYNPD			0x1c // Enable dynamic payload len.
#define FEATURE			0x1d // Feature reg.
/* @} */

/****************************************************/
/*	Bits of resiger
 ****************************************************/

/**
 * @brief Bits in register.
 * @{
 */
/* configuratio nregister */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

/* enable auto acknowledgment */
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

/* enable rx addresses */
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

/* setup of address width */
#define AW          0 /* 2 bits */

/* setup of auto re-transmission */
#define ARD         4 /* 4 bits */
#define ARC         0 /* 4 bits */

/* RF setup register */
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      1 /* 2 bits */   

/* general status register */
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1 /* 3 bits */
#define TX_FULL     0

/* transmit observe register */
#define PLOS_CNT    4 /* 4 bits */
#define ARC_CNT     0 /* 4 bits */

/* fifo status */
#define TX_REUSE    6
#define FIFO_TX_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

/* dynamic length */
#define DPL_P0      0
#define DPL_P1      1
#define DPL_P2      2
#define DPL_P3      3
#define DPL_P4      4
#define DPL_P5      5

/* feature */
#define EN_DPL		2	
#define EN_ACK_PAY	1
#define EN_DNY_ACK	0
/* @} */

/********************************************/
/*	Register's value 
 ********************************************/

#define CONFIG_DEFAULT ((1<<EN_CRC)|(0<<CRCO))

// Address width value.
#define AW_3			1
#define AW_4			2
#define AW_5			3

// Auto retransmission value.
#define AUTO_RETRANS_DELAY(n)	((n-1)<<4)	// unit 250us
#define AUTO_RETRANS_COUNT(n)	(n)

// FIFO status text.
#define fifo_tx_full(status)	(!!(status&(1<<FIFO_TX_FULL)))
#define fifo_tx_empty(status)	(!!(status&(1<<TX_EMPTY)))
#define fifo_rx_full(status)	(!!(status&(1<RX_FULL)))
#define fifo_rx_empty(status)	(status&(1<<RX_EMPTY))

/***************************************************/
/*	Static data
 ***************************************************/
#define NRF24L01P_CONF_AUTO_ACK			(1<<0)
#define NRF24L01P_CONF_STATIC_PAYLOAD_LEN	(1<<1)

static struct struct_nrf24l01p_config{
	uint8_t config;
} nrf24l01p_conf;


/****************************************************/
/*	Configuration related
 ****************************************************/

#if NRF24L01P_CONF_STATE_TRACE
static uint8_t _current_state;
#endif

/**************************************************/
/*	Low-level static function
 **************************************************/
static void nrf24l01p_writeReg( uint8_t reg, uint8_t *data, uint8_t len ){
	nrf24l01p_doCommand( W_REGISTER(reg), data, 0,  len );
}


static void nrf24l01p_singleWriteReg( uint8_t reg, uint8_t data ){
	nrf24l01p_doCommand( W_REGISTER(reg), &data, 0, 1 );
}

static void nrf24l01p_readReg( uint8_t reg, uint8_t *data, uint8_t len ){
	nrf24l01p_doCommand( R_REGISTER(reg), 0, data, len );
}

static uint8_t nrf24l01p_singleReadReg( uint8_t reg ){
	uint8_t data;
	nrf24l01p_doCommand( R_REGISTER(reg), 0, &data, 1 );
	return data;
}

/*****************************************************/
/*	API implements
 *****************************************************/
void nrf24l01p_defaultConfig( void );

void nrf24l01p_init( void ){
	nrf24l01p_platformInit();	
	nrf24l01p_powerDown();
	nrf24l01p_defaultConfig();
#if NRF24L01P_CONF_STATE_TRACE
	_current_state = NRF24L01P_STATE_POWERDOWN;
#endif
}

/**
 * @brief Set to default configuration.
 * Auto ACK		enable 
 * Payload len		static 
 */
void nrf24l01p_defaultConfig( void ){ // TODO

	nrf24l01p_doCommand( FLUSH_RX, 0, 0, 0 );
	nrf24l01p_doCommand( FLUSH_TX, 0, 0, 0 );
	nrf24l01p_singleWriteReg( STATUS, 0xff );


	/**********************/
	nrf24l01p_singleWriteReg( EN_AA, 0x3f ); // enable all AUTO ACK.
	nrf24l01p_singleWriteReg( EN_RXADDR, 0x00 ); // disable all RX addr.
	nrf24l01p_singleWriteReg( SETUP_AW, 3 ); // address width: 5
	nrf24l01p_singleWriteReg( SETUP_RETR, 0x03 );
	nrf24l01p_singleWriteReg( RF_CH, 0x02 );
	nrf24l01p_singleWriteReg( RF_SETUP, 0x0e );

	/* disable dynamic payload len. */
	nrf24l01p_singleWriteReg( FEATURE, 0x00 ); 
	nrf24l01p_singleWriteReg( DYNPD, 0x00 );

	nrf24l01p_conf.config = (
			 NRF24L01P_CONF_AUTO_ACK 
			|NRF24L01P_CONF_STATIC_PAYLOAD_LEN
			);
}

void nrf24l01p_setDynamicPayloadLen( uint8_t enable ){
	nrf24l01p_singleWriteReg( DYNPD, 0x00 );
	if( enable ){
		nrf24l01p_singleWriteReg( FEATURE, (1<<EN_DPL) );
		nrf24l01p_conf.config &= ~(NRF24L01P_CONF_STATIC_PAYLOAD_LEN);
	}
	else{
		nrf24l01p_singleWriteReg( FEATURE, 0x00 );
		nrf24l01p_conf.config |= NRF24L01P_CONF_STATIC_PAYLOAD_LEN;
	}
}

void nrf24l01p_setChannel( uint8_t ch ){
#if NRF24L01P_CONF_STATE_TRACE
	if( _current_state == NRF24L01P_STATE_TX 
		|| _current_state == NRF24L01P_STATE_RX ){
		DEBUG_OUTPUT( "setChannel: State error.\n" );
		return;
	}
#endif
	
	nrf24l01p_singleWriteReg( RF_CH, ch );
}

void nrf24l01p_setTxAddr( uint8_t *addr, uint8_t len ){
#if NRF24L01P_CONF_STATE_TRACE
	if( _current_state == NRF24L01P_STATE_TX 
		|| _current_state == NRF24L01P_STATE_RX ){
		DEBUG_OUTPUT( "setTxAddr function: State error\n" );
		return;
	}
#endif

	/* if auto ack is enabled, must enable rx pipe 0. */
	if( nrf24l01p_conf.config & NRF24L01P_CONF_AUTO_ACK ){
		uint8_t en_rxaddr = nrf24l01p_singleReadReg( EN_RXADDR );
		en_rxaddr |= 0x01;
		nrf24l01p_singleWriteReg( EN_RXADDR, en_rxaddr );
		nrf24l01p_writeReg( RX_ADDR_P0, addr, len );
	}

	/* If dynamic payload len is enabled, DPL_P0 must be set. But why ?*/
	if( !(nrf24l01p_conf.config & NRF24L01P_CONF_STATIC_PAYLOAD_LEN) ){
		nrf24l01p_singleWriteReg( DYNPD, 0x01 );
	}

	nrf24l01p_writeReg( TX_ADDR, addr, 5 );
	/* TODO if using static payload len, this is unnecessary */
}

void nrf24l01p_setRxAddr( uint8_t *addr, uint8_t len, uint8_t pipe, uint8_t payload_len ){
	uint8_t val;

#if NRF24L01P_CONF_STATE_TRACE
	if( _current_state == NRF24L01P_STATE_TX 
		|| _current_state == NRF24L01P_STATE_RX ){
		DEBUG_OUTPUT( "setRxAddr function: State error\n" );
		return;
	}
#endif
	val = nrf24l01p_singleReadReg( EN_RXADDR );
	if( addr == NULL ){
		val &= ~(1<<pipe);
		nrf24l01p_singleWriteReg( EN_RXADDR, val );
	}
		
	else{
		/* enable rx pipe */
		val |= (1<<pipe);
		nrf24l01p_singleWriteReg( EN_RXADDR, val );

		/* Static or dynamic payload len */
		if( nrf24l01p_conf.config & NRF24L01P_CONF_STATIC_PAYLOAD_LEN ){
			nrf24l01p_singleWriteReg( RX_PW_P0+pipe, payload_len );
		}
		else{
			val = nrf24l01p_singleReadReg( DYNPD );
			val |= (1<<pipe);
			nrf24l01p_singleWriteReg( DYNPD, val );
		}

		if( pipe >= 2 ){
			nrf24l01p_writeReg( RX_ADDR_P0+pipe, addr, 1 );
		}
		else{
			/* Set address width. */
			nrf24l01p_singleWriteReg( SETUP_AW, len-2 );
			nrf24l01p_writeReg( RX_ADDR_P0+pipe, addr, len );
		}
	}
}

void nrf24l01p_enterTxMode( void ){
	nrf24l01p_chipEnable( 0 );
	nrf24l01p_doCommand( FLUSH_TX, 0, 0, 0 );
	nrf24l01p_doCommand( FLUSH_RX, 0, 0, 0 );
	nrf24l01p_singleWriteReg( STATUS, 0xff );

	nrf24l01p_singleWriteReg( CONFIG, 0x06 );
	nrf24l01p_chipEnable( 1 );
#if NRF24L01P_CONF_STATE_TRACE
	_current_state = NRF24L01P_STATE_TX;
#endif
}

void nrf24l01p_enterRxMode( void ){
	nrf24l01p_chipEnable( 0 );
	nrf24l01p_doCommand( FLUSH_TX, 0, 0, 0 );
	nrf24l01p_doCommand( FLUSH_RX, 0, 0, 0 );
	nrf24l01p_singleWriteReg( STATUS, 0xff );

	nrf24l01p_singleWriteReg( CONFIG, 0x07 );
	nrf24l01p_chipEnable( 1 );
#if NRF24L01P_CONF_STATE_TRACE
	_current_state = NRF24L01P_STATE_RX;
#endif
}

void nrf24l01p_standby( void ){
	nrf24l01p_chipEnable( 0 );
	nrf24l01p_singleWriteReg( CONFIG, CONFIG_DEFAULT );
#if NRF24L01P_CONF_STATE_TRACE
	_current_state = NRF24L01P_STATE_STANDBY;
#endif
}

void nrf24l01p_powerDown( void ){
	nrf24l01p_chipEnable( 0 );
	nrf24l01p_singleWriteReg( CONFIG, CONFIG_DEFAULT ); 
#if NRF24L01P_CONF_STATE_TRACE
	_current_state = NRF24L01P_STATE_UNKNOWN;
#endif
}

uint8_t	nrf24l01p_transmit( uint8_t *data, uint8_t len ){

	uint8_t status;
	uint8_t ret;

#if NRF24L01P_CONF_STATE_TRACE
	if( _current_state != NRF24L01P_STATE_TX ){
		DEBUG_OUTPUT( "transmit function: State error\n" );
		return 0;
	}
#endif
	/* Check fifo full */
	status = nrf24l01p_readReg( FIFO_STATUS );
	if( fifo_tx_full( status ) ){
		DEBUG_OUTPUT( "transmit error: fifo full\n" );
		return 0;
	}

	nrf24l01p_doCommand( W_TX_PAYLOAD, data, 0, len );
	while( 1 ){
		status = nrf24l01p_singleReadReg( STATUS );
		if( status & (1<<TX_DS) ){
			ret = 1;
			break;
		}
		else if( status & (1<<MAX_RT) ){
			ret = 0;
			break;
		}
	}
	
	nrf24l01p_singleWriteReg( STATUS, 0xff );
	return ret;
}

uint8_t nrf24l01p_dataPending( ){
	uint8_t status;
	uint8_t fifo_status;
	uint8_t pipeno = PIPE_NONE;

#if NRF24L01P_CONF_STATE_TRACE
	if( _current_state != NRF24L01P_STATE_RX ){
		DEBUG_OUTPUT( "dataPending function:State error\n" );	
		return PIPE_NONE;
	}
#endif
	status = nrf24l01p_singleReadReg( STATUS );
	fifo_status = nrf24l01p_singleReadReg( FIFO_STATUS );
	if( status & (1<<RX_DR) ){
		nrf24l01p_singleWriteReg( STATUS, 0xff );
		pipeno = (status&0x0e)>>1;
	}
	else if( !fifo_rx_empty( fifo_status ) ){
		nrf24l01p_singleWriteReg( STATUS, 0xff );
		pipeno = (status&0x0e)>>1; // TODO Is this right pipe number ??
	}
	
	return pipeno;
}

uint8_t nrf24l01p_receive( uint8_t *data, uint8_t maxlen ){
	uint8_t fifo_status;
	uint8_t len;

#if NRF24L01P_CONF_STATE_TRACE
	if( _current_state != NRF24L01P_STATE_RX ){
		DEBUG_OUTPUT( "reveive function:State error\n" );
		return 0;
	}
#endif

	fifo_status = nrf24l01p_singleReadReg( FIFO_STATUS );	
	if( fifo_rx_empty( fifo_status ) ){
		return 0;
	}
	// TODO len
	nrf24l01p_doCommand( R_RX_PL_WID, 0, &len, 1 );
	if( maxlen < len ){
		DEBUG_OUTPUT( "revc buff not enough.\n" );
		len = maxlen;
	}
	nrf24l01p_doCommand( R_RX_PAYLOAD, 0, data, len );
	return len;

}



#if NRF24L01P_CONF_STATE_TRACE
uint8_t nrf24l01p_getState( void ){
	return _current_state;
}
#endif


/************************************************
 *	Test
 ************************************************/
/**
 * @brief Test SPI read and write functions.
 */
void nrf24l01p_test_rw( void ){
	uint8_t data[5] = {0,1,2,3,4};

	DEBUG_OUTPUT( "write data (0x%x,0x%x,0x%x,0x%x,0x%x) to TX_ADDR\n",
			data[0], data[1], data[2], data[3], data[4] );
	nrf24l01p_doCommand( W_REGISTER(TX_ADDR), data, 0, 5 );
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	nrf24l01p_doCommand( R_REGISTER(TX_ADDR), 0, data, 5 );
	DEBUG_OUTPUT( "Result is 0x%x,0x%x,0x%x,0x%x,0x%x\n:", 
			data[0], data[1], data[2], data[3], data[4] );
}

void nrf24l01p_test_tx( void ){
	uint8_t data[4] = { 0x0f,0xf0,0xaa,0 };
	uint8_t addr_p0[5] = {0xe7,0xe7,0xe7,0xe7,0xe7};
	uint8_t addr_p1[5] = {0xc2,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p2[5] = {0xc3,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p3[5] = {0xc4,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p4[5] = {0xc5,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p5[5] = {0xc6,0xc2,0xc2,0xc2,0xc2};
	uint8_t *addrs[6] = { 
				addr_p0,addr_p1,addr_p2,
				addr_p3,addr_p4,addr_p5 };

	//uint8_t addr_p0[5] = {0x11,0x11,0x11,0x11,0x11};
	//uint8_t addr_p1[5] = {0x11,0x11,0x11,0x11,0x12};

	uint8_t pipe = 0;

	nrf24l01p_init();
	nrf24l01p_test_rw();

	while( 1 ){
		nrf24l01p_standby();
		nrf24l01p_setTxAddr( addrs[pipe], 5 );
		if( ++pipe>5 ) pipe = 0;
		nrf24l01p_enterTxMode();

		DEBUG_OUTPUT( "send data: 0x%x,0x%x,0x%x,0x%x\n",
			data[0],data[1],data[2],data[3] );

		if( nrf24l01p_transmit( data, 4 ) ){
			DEBUG_OUTPUT( "send ok\n" );
			data[3] ++;
		}
		else{
			DEBUG_OUTPUT( "send falure\n" );
		}
		nrf24l01p_delay_ms( 500 );
	}
}

void nrf24l01p_test_rx( void ){
	uint8_t data[4];
	uint8_t addr_p0[5] = {0xe7,0xe7,0xe7,0xe7,0xe7};
	uint8_t addr_p1[5] = {0xc2,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p2[1] = {0xc3};
	uint8_t addr_p3[1] = {0xc4};
	uint8_t addr_p4[1] = {0xc5};
	uint8_t addr_p5[1] = {0xc6};
	
	
	//uint8_t addr_p0[5] = {0x11,0x11,0x11,0x11,0x11};
	//uint8_t addr_p1[5] = {0x11,0x11,0x11,0x11,0x12};
	uint8_t len;
	uint8_t pipe;

	nrf24l01p_init();
	nrf24l01p_test_rw();

	nrf24l01p_setRxAddr( addr_p0, 5, PIPE0, 4 );
	nrf24l01p_setRxAddr( addr_p1, 5, PIPE1, 4 );
	nrf24l01p_setRxAddr( addr_p2, 1, PIPE2, 4 );
	nrf24l01p_setRxAddr( addr_p3, 1, PIPE3, 4 );
	nrf24l01p_setRxAddr( addr_p4, 1, PIPE4, 4 );
	nrf24l01p_setRxAddr( addr_p5, 1, PIPE5, 4 );

	nrf24l01p_enterRxMode();

	while( 1 ){
		if( (pipe=nrf24l01p_dataPending()) != PIPE_NONE ){
			len = nrf24l01p_receive( data, 4 );
			if( len == 0 ){
				DEBUG_OUTPUT( "error, no data\n" );
			}else
				DEBUG_OUTPUT( "pipe %u data: 0x%x,0x%x,0x%x,0x%x\n", 
					pipe, data[0],data[1],data[2],data[3] );
		}
	}
}

void nrf24l01p_test_dpl_tx( void ){
	uint8_t data[6] = { 0x00,0x00,0x00,0x00,0x00,0x00 };
	uint8_t addr_p0[5] = {0xe7,0xe7,0xe7,0xe7,0xe7};
	uint8_t addr_p1[5] = {0xc2,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p2[5] = {0xc3,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p3[5] = {0xc4,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p4[5] = {0xc5,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p5[5] = {0xc6,0xc2,0xc2,0xc2,0xc2};
	uint8_t *addrs[6] = { 
				addr_p0,addr_p1,addr_p2,
				addr_p3,addr_p4,addr_p5 };

	//uint8_t addr_p0[5] = {0x11,0x11,0x11,0x11,0x11};
	//uint8_t addr_p1[5] = {0x11,0x11,0x11,0x11,0x12};

	uint8_t pipe = 5;

	nrf24l01p_init();
	nrf24l01p_test_rw();
	nrf24l01p_setDynamicPayloadLen( 1 );

	while( 1 ){
		nrf24l01p_standby();
		if( ++pipe>5 ) pipe = 0;
		nrf24l01p_setTxAddr( addrs[pipe], 5 );
		nrf24l01p_enterTxMode();

		DEBUG_OUTPUT( "send data to pipe %d\n", pipe );

		if( nrf24l01p_transmit( data, pipe+1 ) ){
			DEBUG_OUTPUT( "send ok\n" );
			data[pipe] ++;
		}
		else{
			DEBUG_OUTPUT( "send falure\n" );
		}
		//nrf24l01p_delay_ms( 500 );
	}
}

void nrf24l01p_test_dpl_rx( void ){
	uint8_t data[10];
	uint8_t addr_p0[5] = {0xe7,0xe7,0xe7,0xe7,0xe7};
	uint8_t addr_p1[5] = {0xc2,0xc2,0xc2,0xc2,0xc2};
	uint8_t addr_p2[1] = {0xc3};
	uint8_t addr_p3[1] = {0xc4};
	uint8_t addr_p4[1] = {0xc5};
	uint8_t addr_p5[1] = {0xc6};
	
	
	//uint8_t addr_p0[5] = {0x11,0x11,0x11,0x11,0x11};
	//uint8_t addr_p1[5] = {0x11,0x11,0x11,0x11,0x12};
	uint8_t len;
	uint8_t pipe;
	uint8_t i;

	nrf24l01p_init();
	nrf24l01p_setDynamicPayloadLen( 1 );
	nrf24l01p_test_rw();

	nrf24l01p_setRxAddr( addr_p0, 5, PIPE0, 0 );
	nrf24l01p_setRxAddr( addr_p1, 5, PIPE1, 0 );
	nrf24l01p_setRxAddr( addr_p2, 1, PIPE2, 0 );
	nrf24l01p_setRxAddr( addr_p3, 1, PIPE3, 0 );
	nrf24l01p_setRxAddr( addr_p4, 1, PIPE4, 0 );
	nrf24l01p_setRxAddr( addr_p5, 1, PIPE5, 0 );

	nrf24l01p_enterRxMode();

	while( 1 ){
		if( (pipe=nrf24l01p_dataPending()) != PIPE_NONE ){
			len = nrf24l01p_receive( data, 10 );
			if( len == 0 ){
				DEBUG_OUTPUT( "error, no data\n" );
			}else{
				DEBUG_OUTPUT( "pipe %u data %u: ", pipe, len );
				for( i=0; i<len; ++i ){
					DEBUG_OUTPUT( "0x%x ", data[i] );	
				}
				DEBUG_OUTPUT( "\n" );
			}
		}
	}
}
void nrf24l01p_test( void ){
	nrf24l01p_init();
	nrf24l01p_test_rw();
}
