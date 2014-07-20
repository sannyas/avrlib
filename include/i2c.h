/**
 * @file i2c.h
 */

#ifndef _I2C_H_
#define _I2C_H_
#include <stdint.h>
#include <twi.h>

/**
 * @addtogroup grp_avr AVR
 * @{
 *
 */

/**
 * @addtogroup grp_i2c I2C
 * @{
 */

#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

/** @brief write a byte data using i2c
 *
 * @para dev_addr	7-bit device address
 * @para data_addr	The data address in this device.
 * @para data		The data to write
 */
uint8_t	i2c_writeByte( uint8_t dev_addr, uint8_t data_addr, uint8_t data  );

/** @brief write a array of data using i2c
 *
 * @para dev_addr	7-bit device address.
 * @para data_addr	The data address in this device.
 * @para data		A array of data to write
 */
uint8_t	i2c_writeBytes( uint8_t dev_addr, uint8_t data_addr, uint8_t *data, uint8_t data_len );

/** @briief read a bity data using i2c
 *
 * @return The data.
 * @para dev_addr	7-bit device address
 * @para data_addr	The data address in this device.
 */
uint8_t	i2c_readByte( uint8_t dev_addr, uint8_t data_addr );

/** @brief read a array of data using i2c.
 *
 * @return The number of data.
 * @para dev_addr	7-bit device address
 * @para data_addr	The data address in this device.
 * @data		The buffer.
 * @data_len		The number of data want to read.
 */
uint8_t	i2c_readBytes( uint8_t dev_addr, uint8_t data_addr, uint8_t *data, uint8_t data_len );

#endif /*  __AVR_ATmega16__, __AVR_ATmega16A__ */

/** @} */
/** @} */

#endif /* _I2C_H_ */

