#include <stdio.h>
#include <usart.h>
#include "../mfrc522.h"

uint8_t func_call_chain_cnt = 0;

int main( void ){
	usart_init();
	stdout = &usart_file;
	stdin = &usart_file;
	printf( "main start\n" );

	mfrc522_test();

	return 0;
}

