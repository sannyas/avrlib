#include <stdio.h>
#include <usart.h>
#include "sdcard.h"

uint8_t buff[128];

uint8_t func_call_chain_cnt = 0;
int main( void ){
	uint8_t resp;
	uint8_t idx;
	uint8_t ret;
	uint8_t j;
	usart_init();
	stdout = &usart_file;

	printf( "main start\n" );
	resp = sd_init();

	/*
	if( 1 ){
		sd_read( resp, 0, buff, 32 );
		sd_read( resp, 0, buff, 32 );
		sd_read( resp, 0, buff, 32 );
	}
	*/
	if( resp == SDCARD_TYPR_1 || resp == SDCARD_TYPR_SDSC ){
		for( idx=0; idx<16; ++idx ){
			ret = sd_read( resp, idx*32, buff, 32 );
			if( ret != SDCARD_FAILURE ){
				for( j=0; j<32; ++j )
					printf( "%x ", buff[j] );
				printf( "\n" );
			}
		}
	}
	else if( resp == SDCARD_TYPR_SDHC ) {
		for( idx=0; idx<16; ++idx ){
			ret = sd_read( resp, idx*2, buff, 32 );
			if( ret != SDCARD_FAILURE ){
				for( j=0; j<32; ++j )
					printf( "%x ", buff[j] );
				printf( "\n" );
			}
		}
	}

	printf( "main end\n" );
	while( 1 );

	return 0;
}
