#ifndef _H_PCF8591_
#define _H_PCF8591_

#include <stdint.h>
/** 
 * @brief Hardware slaver address.
 * 
 * This macro define a 3-bit address of slaver 
 * address. these bits are defined by connect the
 * input pins A0 to A2 to either Vss or Vdd.
 */
#define PCF8591_CFG_I2C_ADDR	0x00 /* 0-7 */

/* I2C slaver address */
#define PCF8591_I2C_ADDR	(0x48+PCF8591_CFG_I2C_ADDR)
#define PCF8591_I2C_ADDR_W	((PCF8591_I2C_ADDR<<1))
#define PCF8591_I2C_ADDR_R	((PCF8591_I2C_ADDR<<1)+1)

/**
 * @name control_byte
 *
 * Control byte description.
 * @{
 */
#define PCF8591_CHANNEL_1	0x00
#define PCF8591_CHANNEL_2	0x01
#define PCF8591_CHANNEL_3	0x02
#define PCF8591_CHANNEL_4	0x03
#define PCF8591_AUTO_INC	0x04	/**< auto increment flag */
/* analog input programming */
#define PCF8591_SINGLE_ENDED	0x00	/**< four single-ended inputs  */
#define PCF8591_THREE_DIFF	0x10	/**< three defferential inputs  */
#define PCF8591_S_D_MIXED	0x20	/**< single-ended and differential mixed */
#define PCF8591_TWO_DIFF	0x30	/**< two differential input */
#define PCF8591_ANALOG_OUTPUT	0x40	/**< analog output enable flag */
/*@}*/

uint8_t pcf8591_conversion( uint8_t channel );
void    pcf8591_analogOutput( uint8_t value );

#endif
