#include <stdio.h>
#include <usart.h>
#include <util/delay.h>
#include <ili9325.h>
#include <avr/interrupt.h>
#include "../xpt2046.h"

void drawPoint( uint16_t x, uint16_t y ){
	if( x<238 && y<318 ){
		ili9325_setWindowArea( x, y, x+2, y+2 );
		ili9325_fillWindow( ILI9325_BLUE );
	}
}

void penirq_callback( void ){
	xpt2046_position_t pos;
	xpt2046_getTouchPos( &pos );
	printf( "%d,%d--", pos.x, pos.y );
	xpt2046_getScreenPos( &pos );
	printf( "%d,%d\n", pos.x, pos.y );
	drawPoint( pos.x, pos.y );
}

int main( void ){
	xpt2046_position_t pos;
	usart_init();	
	stdout = &usart_file;
	printf( "main start\n" );
	
	ili9325_init();
	ili9325_clearScreen( ILI9325_YELLOW );
	 
	xpt2046_init();
	xpt2046_penIrqRegister( penirq_callback );
	xpt2046_setScreenSize( 240, 320 );
	

	xpt2046_startAdjust( drawPoint );
	ili9325_clearScreen( ILI9325_YELLOW );
	sei();
	
	while( 1 ){
	}

	printf( "main end\n" );
	return 0;
}
