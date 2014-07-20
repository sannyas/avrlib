#include <stdint.h>
#include <util/delay.h>
#include "sdcard.h"
#include "cmd.h"

#ifdef SDCARD_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#define RETRY_RESP_COUNT	200
#define RETRY_CMD_COUNT		200

/* Global variable */
uint8_t resp_data[4];

uint8_t sd_spi_rw( uint8_t data ){
	uint8_t idx;
	for( idx=0; idx<8; ++idx ){
		sd_pin_DI_setValue( data&0x80 );
		data <<= 1;
		sd_pin_SCLK_setValue( 1 );
		if( sd_pin_DO_getValue() )
			data |= 0x01;
		sd_pin_SCLK_setValue( 0 );
	}
	return data;
}

void sd_clockDelay( uint8_t cnt ){
	while( cnt-- )
		sd_spi_rw( 0xff );
}

void sd_readBuffer( uint8_t *buff, uint16_t len ){
	//sd_pin_CS_setValue( 0 );

	if( buff ){
		while( len-- ){
			*buff++ = sd_spi_rw( 0xff );
		}
	}
	else{
		while( len-- )
			sd_spi_rw( 0xff );
	}
	//sd_pin_CS_setValue( 1 );
}

uint8_t __sd_sendCommand( uint8_t cmd, uint32_t arg ){
	sd_spi_rw( cmd | 0x40 );
	sd_spi_rw( arg>>24 );
	sd_spi_rw( arg>>16 );
	sd_spi_rw( arg>>8 );
	sd_spi_rw( arg );

	/*
	 * 在SPI模式CRC默认不使能, 但CMD8命令需要CRC
	 * CMD8命令参数为0x1aa的CRC为0x87
	 */
	if( cmd != CMD8 )
		sd_spi_rw( 0x95 );
	else 
		sd_spi_rw( 0x87 );
}
uint8_t sd_sendCommand( uint8_t cmd, uint32_t arg ){
	uint8_t resp;
	uint8_t retry;
	uint8_t temp;

	sd_clockDelay( 2 );
	sd_pin_CS_setValue( 0 );
	__sd_sendCommand( cmd, arg );

	retry = RETRY_RESP_COUNT;
	resp = 0xff;
	while( resp & 0x80 ){
		if( --retry == 0 )
			break;
		temp = sd_spi_rw( 0xff );
		if( temp != resp ){
			PDEBUG( "-- resp : 0x%x\n", temp );
		}
		resp = temp;
	}
	PDEBUG( "--\n" );

	if( (resp&0x80) == 0 ){
		if( cmd == CMD8 || cmd == CMD58 ){
			resp_data[0] = sd_spi_rw( 0xff );
			resp_data[1] = sd_spi_rw( 0xff );
			resp_data[2] = sd_spi_rw( 0xff );
			resp_data[3] = sd_spi_rw( 0xff );
		}
	}
	sd_pin_CS_setValue( 1 );
	sd_clockDelay( 2 );

	return resp;
}


uint8_t __sd_init( void ){
	uint16_t retry;
	uint8_t resp;

	sd_pin_setup();
	sd_clockDelay( 10 ); // 80 clock
	//sd_pin_CS_setValue( 0 );

	/* 发送CMD0, 进入SPI模式 */
	retry = RETRY_CMD_COUNT;
	do{
		resp = sd_sendCommand( CMD0, 0 );
		if( --retry == 0 ) break;
	}while( resp != IN_IDLE_STATE );
	sd_clockDelay( 2 );
	PDEBUG( "CMD0 response 0x%x\n", resp );
	if( resp != IN_IDLE_STATE ){
		PDEBUG( "CMD0 failure\n" );
		return SDCARD_FAILURE;
	}

	/* 
	 * 发送CMD8命令, 1.x的卡不支持CMD8命令, 对于2.0卡
	 * 该命令是必要的, 可用于判断卡的版本
	 */ 
	retry = RETRY_CMD_COUNT;
	do{
		resp = sd_sendCommand( CMD8, 0x1aa );
		if( --retry == 0 ) break;
	}while( resp == 0xff );
	sd_clockDelay( 2 );
	PDEBUG( "CMD8 response: 0x%x\n", resp );
	if( resp == 0xff ){
		PDEBUG( "CMD8 failure\n" );
		return SDCARD_FAILURE;
	}

	/*
	 * 如果响应CMD8, 则为2.00卡
	 */
	if( resp == IN_IDLE_STATE ){ 
		PDEBUG( "This is V2.00 Card\n" );
		//sd_readBuffer( resp_data, 4 );
		PDEBUG( "resp data = 0x%x, 0x%x, 0x%x, 0x%x\n", resp_data[0], resp_data[1],resp_data[2],resp_data[3] );
		if( resp_data[2] == 0x01 && resp_data[3] == 0xaa ){
		//if( 1 ){ /* TODO */
			/*
			 * 发送ACMD41
			 */
			retry = RETRY_CMD_COUNT;
			do{
				resp = sd_sendCommand( CMD55, 0 );
				sd_clockDelay( 2 );
				resp = sd_sendCommand( ACMD41, 0x40000000 );
				if( --retry == 0 ) break;
			}while( resp != 0x00 );
			sd_clockDelay( 2 );
			PDEBUG( "ACMD41 response : 0x%x\n", resp );
			if( resp != 0x00 ){
				PDEBUG( "ACMD41 failure\n" );
				return SDCARD_FAILURE;
			}
			PDEBUG( "ACMD41 OK\n" );

			/*
			 * 发送CMD58, read ocr
			 */
			retry = RETRY_CMD_COUNT;
			do{
				resp = sd_sendCommand( CMD58, 0 ); // 返回R3
				if( --retry == 0 ) break;
			}while( resp != 0x00 );
			PDEBUG( "CMD58 response : 0x%x\n", resp );
			PDEBUG( "resp data = 0x%x\n", resp_data[0] );
			if( resp_data[0]&OCR_CCS_BYTE ){
				PDEBUG( "SDHC or SDXC\n" );
				return SDCARD_TYPR_SDHC;
			}
			else{
				PDEBUG( "SDSC\n" );
				return SDCARD_TYPR_SDSC;
			}



		}else{
			PDEBUG( "CMD8 response error\n" );
			return SDCARD_FAILURE;
		}
	}

	/*
	 * 如果不能正确响应CMD8, 则为1.x卡
	 */
	//else if( resp == (IN_IDLE_STATE|ILLEGAL_COMMAND) ){ // V 1.x
	else{
		PDEBUG( "This is V1.x Card\n" );
		/*
		 * 发送ACMD41
		 */
		retry = RETRY_CMD_COUNT;
		do{
			resp = sd_sendCommand( CMD55, 0 );
			resp = sd_sendCommand( ACMD41, 0 );
			if( --retry == 0 ) break;
		}while( resp != 0x00 );
		PDEBUG( "ACMD41 response : 0x%x\n", resp );
		if( resp != 0x00 ){
			PDEBUG( "ACMD41 failure\n" );
			return SDCARD_FAILURE;
		}
		PDEBUG( "ACMD41 OK\n" );

		return SDCARD_TYPR_1;
	}


	PDEBUG( "init OK\n" );
	return SDCARD_SUCCESS;
}

uint8_t sd_init( void ){
	uint8_t ret;
	ret = __sd_init( );
	if( ret == SDCARD_FAILURE )
		ret = __sd_init();
	if( ret != 2 )
		PDEBUG( "Success: Init Card\n" );
	return ret;
}

uint8_t sd_read( uint8_t card_type, uint32_t addr, uint8_t *buff, uint16_t size ){
	uint32_t block_addr;
	uint16_t offset;
	uint8_t resp;
	uint8_t retry;
	uint16_t idx;

	if( card_type == SDCARD_TYPR_SDSC || card_type == SDCARD_TYPR_1 ){
		block_addr = addr &(~0x1ff);
		offset = addr & 0x1ff;
	}
	else{
		block_addr = addr>>5;
		offset = ( addr - (block_addr<<5) ) * 16;
	}
	PDEBUG( "read block addr %u\n", block_addr );
	PDEBUG( "offset %u\n", offset );

	resp = sd_sendCommand( READ_SINGLE_BLOCK, block_addr );
	if( resp != 0x00 ){
		PDEBUG( "CMD READ_SINGLE_BLOCK failure: %d\n", resp );
		return SDCARD_FAILURE;
	}

	/* Wait for token 0xfe */
	sd_pin_CS_setValue( 0 );
	retry = RETRY_CMD_COUNT;
	do{
		resp = sd_spi_rw( 0xff );
		if( retry-- == 0 ){
			sd_pin_CS_setValue( 1 );
			PDEBUG( "wait for token: timeout\n" );
			return SDCARD_FAILURE;
		}
	}while( resp != 0xfe );


	sd_readBuffer( 0, offset );	
	sd_readBuffer( buff, size );
	sd_readBuffer( 0, 514-size-offset );
	sd_pin_CS_setValue( 1 );
	sd_clockDelay( 10 );
	

	/*
	retry = 0;
	for( idx=0; idx<512; ++idx ){
		printf( "%x ", sd_spi_rw( 0xff ) );
		++retry;
		if( retry == 32 ){
			retry = 0;
			printf( "\n" );
		}
	}
	sd_spi_rw( 0xff );
	sd_spi_rw( 0xff );
	sd_spi_rw( 0xff );
	sd_pin_CS_setValue( 1 );
	*/

	//_delay_ms( 1000 );
	PDEBUG( "Read OK\n" );
}
