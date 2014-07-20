#include <avr/io.h>
#include "boardled.h"

void boardled_init( void ){
	DDRA |= _BV(PA0);
}

void boardled_on( void ){
	PORTA &= ~_BV(PA0);
}

void boardled_off( void ){
	PORTA |= _BV(PA0);
}

void boardled_ch( void ){
	PORTA ^= _BV(PA0);
}


