#include <avr/io.h>
#include "led.h"

void led_init( void ){
	DDRA |= 0x01; // output
	PORTA |= 0x00;
	TCCR0 = 0;
}

static uint8_t led_status = 0;
void led_do( void ){
	if( led_status == 0 ){
		PORTA &= 0xfe;
		led_status = 1;
	}
	else{
		PORTA |= 0x01;
		led_status = 0;
	}
}
