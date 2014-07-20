#ifndef _H_SDCARD_
#define _H_SDCARD_

#define SDCARD_SUCCESS	1
#define SDCARD_FAILURE	2

/**
 * @brief SD card Type
 * 
 * @{
 */
#define SDCARD_TYPR_1		10 /**< Version 1.00 */
#define SDCARD_TYPR_SDHC	12 /**< High Capacity Card*/
#define SDCARD_TYPR_SDSC	13 /**< Standard Capacity Card */
/* @} */

/**
 * @brief  SD card initialization
 *
 * @ret SDCARD_FAILURE Initialization failure.
 * @ret SDCARD_TYPE_*  If successful, return the card type.
 */
uint8_t sd_init( void );

/**
 * @brief Read data from SD card
 *
 * 如果是标准容量卡, 以字节为单位
 * 如果是大容量卡, 以16字节为单位
 */
uint8_t sd_read( uint8_t card_type, uint32_t addr, uint8_t *buff, uint16_t size );
void sd_test( void );


/*************************
 * pins operations
 *************************/
void    sd_pin_setup( void );
void    sd_pin_SCLK_setValue( uint8_t v );
void    sd_pin_CS_setValue( uint8_t v );
void    sd_pin_DI_setValue( uint8_t v ); /* MOSI */
uint8_t sd_pin_DO_getValue( void ); /* MISO */
#endif
