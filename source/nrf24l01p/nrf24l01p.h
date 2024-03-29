#ifndef _H_NRF24L01PLUS_
#define _H_NRF24L01PLUS_

#include <stdint.h>


/*
 * Configurations
 */
#define NRF24L01P_CONF_STATE_TRACE		0

/* PIPE number */
#define PIPE0			0
#define PIPE1			1
#define PIPE2			2
#define PIPE3			3
#define PIPE4			4
#define PIPE5			5
#define PIPE_NONE		7

/**
 * @brief API
 * @{
 */
void 	nrf24l01p_init( void );

void	nrf24l01p_setDynamicPayloadLen( uint8_t enable );
void	nrf24l01p_setChannel( uint8_t ch );

void	nrf24l01p_setTxAddr( uint8_t *addr, uint8_t len );
void	nrf24l01p_setRxAddr( uint8_t *addr, uint8_t len, uint8_t pipe, uint8_t payload_len );

void	nrf24l01p_enterTxMode( void );
void	nrf24l01p_enterRxMode( void );
void	nrf24l01p_standby( void );
void	nrf24l01p_powerDown( void );

uint8_t	nrf24l01p_transmit( uint8_t *data, uint8_t len );
uint8_t nrf24l01p_dataPending( void );
uint8_t nrf24l01p_receive( uint8_t *data, uint8_t maxlen );
/* @} */

/**
 * @brief Other methods decided by CONFIG.
 * @{
 */

/* State trace. */
#if NRF24L01P_CONF_STATE_TRACE
  #define NRF24L01P_STATE_UNKNOWN	(1)
  #define NRF24L01P_STATE_POWERDOWN	(2)
  #define NRF24L01P_STATE_STANDBY	(3)
  #define NRF24L01P_STATE_TX		(4)
  #define NRF24L01P_STATE_RX		(5)
  uint8_t nrf24l01p_getState( void );
#endif

/* @} */

// For test.
void nrf24l01p_test( void );
void nrf24l01p_test_tx();
void nrf24l01p_test_rx();
void nrf24l01p_test_dpl_tx();
void nrf24l01p_test_dpl_rx();



/**
 * These functions is implementation-dependent, which should
 * be rewrite in different platform.
 *
 * @{
 */
#include "nrf24l01p_platform.h"

/**
 * @brief Execute command.
 *
 * @param cmd	Command code, see header file.
 * @param data	This is a result-value parameter.
 * @param len	The length of data.
 */
extern void nrf24l01p_doCommand( uint8_t cmd, uint8_t const *data_in, uint8_t *data_out, uint8_t len );

/**
 * @brief Set Chip Enable.
 * 
 * @param en	0 or 1.
 */
extern void nrf24l01p_chipEnable( uint8_t enable );




/**
 * @brief Platform-dependent initialization, about SPI interface.
 */
extern void nrf24l01p_platformInit( void );
#ifndef nrf24l01p_delay_us
extern void nrf24l01p_delay_us( uint16_t n );
#endif
#ifndef nrf24l01p_delay_ms
extern void nrf24l01p_delay_ms( uint16_t n );
#endif

#endif
