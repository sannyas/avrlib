#ifndef __H_ENC28J60__
#define __H_ENC28J60__

#include <stdint.h>

/* ****************************** */ 
/* Configuration */
/* ****************************** */ 
#define ENC28J60_CFG_TX_BUFF_START	0x0000
#define ENC28J60_CFG_RX_BUFF_START	0x0600
#define ENC28J60_CFG_RX_BUFF_STOP	0x1fff
#define ENC28J60_CFG_MAX_FRAME_LEN	1518
//#define ENC28J60_CFG_FULL_DUPLEX	0
//#define ENC28J60_CFG_USE_INTERRUPT	1


/* ****************************** */
// Clock output config
/* ****************************** */

/**
 * @name clkout_cfg
 * Clock output configure. The base clock
 * is 25 MHz. The frequency of clock output
 * is this base clock frequency divided by
 * specific prescale value.
 *
 * @{
 */
#define ENC28J60_CLKOUT_DISABLE	0x00 /**< Disable clock output */
#define ENC28J60_CLKOUT_DIV_1	0x01 /**< main clock divided by 1 (25 MHz) */
#define ENC28J60_CLKOUT_DIV_2	0x02 /**< main clock divided by 2 (12.5 MHz) */
#define ENC28J60_CLKOUT_DIV_3	0x04 /**< main clock divided by 3 (8.33 MHz) */
#define ENC28J60_CLKOUT_DIV_4	0x04 /**< main clock divided by 4 (6.25 MHz) */
#define ENC28J60_CLKOUT_DIV_8	0x05 /**< main clock divided by 8 (3.125 MHz) */
/*@}*/
/**
 * @brief Clock output configure function.
 *
 * This function is used to configure clock
 * outpu in CLKOUT pin.
 * @param cfg The value of prescale, ENC28J60_CLKOUT_*
 */
void enc28j60_clkoutConfig( uint8_t cfg );


/* ****************************** */
// LED config
/* ****************************** */
/* TODO
#define ENC28J60_LED_TRANSMIT			0x01
#define ENC28J60_LED_RECEIVE			0x02
#define ENC28J60_LED_COLLISION			0x03
#define ENC28J60_LED_LINK			0x04
#define ENC28J60_LED_DUPLEX			0x05
#define ENC28J60_LED_TRANS_AND_RECV		0x07
#define ENC28J60_LED_ON				0x08
#define ENC28J60_LED_OFF			0x09
#define ENC28J60_LED_BLINK_FAST			0x0a
#define ENC28J60_LED_BLINK_SLOW			0x0b
#define ENC28J60_LED_LINK_AND_RECV		0x0c
#define ENC28J60_LED_LINK_AND_TRANS		0x0d
#define ENC28J60_LED_DUPLEX_AND_COLLISION	0x0e

#define ENC28J60_LED_PULSE_STRETCH_40ms		(0x00<<2)
#define ENC28J60_LED_PULSE_STRETCH_73ms		(0x01<<2)
#define ENC28J60_LED_PULSE_STRETCH_139ms	(0x02<<2)

#define ENC28J60_LED_PULSE_STRETCH_ENABLE	(0x01<<1)
#define ENC28J60_LED_PULSE_STRETCH_DISABLE	(0x00<<1)

void enc28j60_ledAConfig( uint8_t conf );
void enc28j60_ledBConfig( uint8_t conf );
void enc28j60_ledPulseStretchConfig( uint8_t conf );
*/

/* ****************************** */
// Receive filter 
/* ****************************** */
/*  TODO
#define ENC28J60_FILTER_AND		0x40
#define ENC28J60_FILTER_OR		0x00
#define ENC28J60_FILTER_UNICAST		0x80
#define ENC28J60_FILTER_PATTERN_MATCH	0x10
#define ENC28J60_FILTER_MAGIC_PACKET	0x08
#define ENC28J60_FILTER_HASH_TABLE	0x04
#define ENC28J60_FILTER_MULTICAST	0x02
#define ENC28J60_FILTER_BROADCAST	0x01
#define ENC28J60_FILTER_CRC		0x02
void enc28j60_recvFilterConfig( uint8_t conf );
*/



/* *********************** */
// Low-level operations
/* *********************** */
/**
 * @name low_level_op
 * These functions do some low-level operations,
 * including reading and writing register.
 * @{
 */
void 	enc28j60_selectBank( uint8_t bank );
uint8_t enc28j60_readCtrlReg( uint8_t reg );
void 	enc28j60_writeCtrlReg( uint8_t reg, uint8_t data );
void 	enc28j60_bitFieldSet( uint8_t reg, uint8_t mask );
void 	enc28j60_bitFieldClear( uint8_t reg, uint8_t mask );
void 	enc28j60_readBufferMem( uint8_t *data, uint8_t len );
void 	enc28j60_writeBufferMem( uint8_t *data, uint8_t len );
void 	enc28j60_writePhy( uint8_t addr, uint16_t data );
uint16_t enc28j60_readPhy( uint8_t addr );
void	enc28j60_softReset( void );
/*@}*/


/* ****************************** */
// System reset
/* ****************************** */
/**
 * @name reset_funcs
 * These functions is used to reset chip or 
 * sub module.
 * @{
 */
void enc28j60_hardReset( void );
void enc28j60_transmitReset( void );
void enc28j60_receiveReset( void );
void enc28j60_phyReset( void );
void enc28j60_macReset( void );
/*@}*/


/**
 * @brief Initialzie
 *
 * Initialzie this chip.
 * @param mac_addr The pointer to MAC address.
 */
void enc28j60_init( uint8_t *mac_addr );

/**
 * @name trans_cfg
 * These macros define the control byte for transmision. 
 * If use default configuration defined in MACON3 in a 
 * transmision, The new_cfg parameter of `enc28j60_transmitPackage`
 * function should be set to ENC28J60_TRANS_USE_DEFAULT.
 * If want to override default configuration, The parameter
 * should be set to ENC28J60_TRANS_USE_NEW OR ENC28J60_TRANS_*
 * @{
 */
#define ENC28J60_TRANS_HUGE_FRAME	0x08
#define ENC28J60_TRANS_PADDING		0x04
#define ENC28J60_TRANS_CRC		0x02
#define ENC28J60_TRANS_USE_NEW		0x01
#define ENC28J60_TRANS_USE_DEFAULT	0x00
/*@}*/ 

/** 
 * @brief Transmit packet
 *
 * Transmit a packet
 * @param data [in] The data to transmit
 * @param len [in] The length of data
 * @new_conf [in] The Control byte for this transmitision
 */
void enc28j60_transmitPacket( uint8_t *data, uint16_t len, uint8_t new_conf );

/** 
 * @brief Receive packet 
 *
 * @param data [out] The memory to save received packet.
 * @max_len [in] The max length of data.
 * @return The length of received data.
 */
uint16_t enc28j60_receivePacket( uint8_t *data, uint16_t max_len );



#if 0
/* ****************************** */
// Old api
/* ****************************** */
void nicSetMacAddress(void);
void encInit( uint8_t *mac_addr );
void encPhyWrite(unsigned char add, unsigned int data);
void encPacketSend( uint8_t *data, uint8_t len );
unsigned int encPacketReceive( uint8_t *data );
#endif


/* ****************************** */
// Pins operations
/* ****************************** */
/**
 * @name pin_ops
 *
 * @{
 */
extern void enc28j60_pin_setup();
extern void enc28j60_pin_CS_setValue( uint8_t v );
extern void enc28j60_pin_SCK_setValue( uint8_t v );
extern uint8_t enc28j60_pin_SO_getValue( void );
extern void enc28j60_pin_SI_setValue( uint8_t v );
extern void enc28j60_pin_RESET_setVakue( uint8_t v );
/*@}*/

#endif
