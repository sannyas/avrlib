#include <stdio.h>
#include <stdint.h>
#include <usart.h>
#include <ioport.h>
#include <util/delay.h>
#include "../ili9325.h"

#define W 240
#define H 320
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0

int main( void ){
	uint16_t w,h;
	uint16_t idx;
	usart_init();	
	stdout = &usart_file;
	printf( "main start\n" );


	/* ili9325 test */
	ili9325_init();
	ili9325_clearScreen( ILI9325_YELLOW );
	ili9325_setWindowArea( 50, 50, 200, 200 );
	ili9325_fillWindow( ILI9325_BLUE );

	printf( "main exit\n" );
	while( 1 ){
	}
	return 0;
}
