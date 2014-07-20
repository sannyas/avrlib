#include <stdio.h>
#include <ioport.h>
#include <util/delay.h>
#include "ili9325.h"


#define ILI9325_WIDTH 240
#define ILI9325_HEIGHT 320
#define ili9325_delay_ms(n) _delay_ms(n)
#define ili9325_delay_us(n) _delay_us(n)

/* Global */
static uint16_t ili9325_width;
static uint16_t ili9325_height;

/* i80 8-bit system bus interface */


/**
 * \brief Write a 8-bit data to device.
 */
static void __ili9325_write8( uint8_t data ){
	/* 
	 * Wtite low-level pulse width: min 50 ns
	 * Write hignh-level pulse width: min 50 ns
	 * Write data set up time: min 10 ns
	 * Write data hold time: min 10 ns
	 */ 
	ili9325_pin_DB_setValue( data );
	ili9325_pin_WR_setValue( 0 );
	/* delay min:  50 ns */
	ili9325_pin_WR_setValue( 1 );
	/* delay min 50 ns */

}

/**
 * \brief Read a 8-bit data from DB
 *
 * This function just pull down and up the 
 * RD signal, and return data receiived from 
 * DB. So before using this function, Must 
 * set DB pins to input mode.
 */
static uint8_t __ili9325_read8( void ){
	/*
	 * Read low-level pulse width: min 150 ns
	 * Read high-level pulse width: min 150 ns
	 * Read data delay time: max 100 ns
	 * Read data hold time: min 5 ns
	 */
	uint8_t data;
	ili9325_pin_RD_setValue( 0 );
	/* delay 100 ns */
	data = ili9325_pin_DB_getValue();
	/* delay 50 ns */
	ili9325_pin_RD_setValue( 1 );
	/* delay 150 ns */
	return data;
}

/**
 * \brief Write a 16-bits data to device.
 */
static void __ili9325_write16( uint16_t data ){
	__ili9325_write8( (uint8_t)(data>>8) );
	__ili9325_write8( (uint8_t)(data&0xff) );
}

/**
 * \brief Read a 16-bit data from device
 */
static uint16_t __ili9325_read16( void ){
	uint16_t data;
	data = __ili9325_read8() << 8;
	data += __ili9325_read8();
	return data;
}

/**
 * \brief Write reg index to IR regisster.
 */
void ili9325_selectReg( uint16_t reg ){

	ili9325_pin_CS_setValue( 0 );
	/* TODO */

	ili9325_pin_RS_setValue( 0 );
	__ili9325_write16( reg );
	ili9325_pin_RS_setValue( 1 );

	ili9325_pin_CS_setValue( 1 );
	/* TODO */
}

/** 
 * \brief Write data with the complete timing sequence
 */
void ili9325_writeData( uint16_t data ){
	ili9325_pin_CS_setValue( 0 );
	/* TODO */
	__ili9325_write16( data );
	ili9325_pin_CS_setValue( 1 );
	/* TODO */
}


/**
 * \brief Write a 16-bit data to specified register 
 *
 * @para index The index of register
 * @para data Tha data to be written
 * */
void ili9325_writeReg( uint8_t index, uint16_t data ){
	/* 
	 * Setup time write( RS to nCS,nWR ): min 10 ns
	 * Setup time read( RS to nCS,nWR,nRD ): min 5 ns
	 *
	 */

	ili9325_selectReg( index );
	ili9325_writeData( data );
}

/**
 * \breif Read data from specified register
 */
uint16_t ili9325_readReg( uint8_t index ){
	uint16_t data;

	ili9325_selectReg( index );

	ili9325_pin_DB_setInMode();
	ili9325_delay_us( 5 );
	/* TODO */
	data = __ili9325_read16( );
	ili9325_pin_DB_setOutMode();

	return data;
}

/**
 * \brief Get device code
 *
 * @return 0x9325
 */
uint16_t ili9325_getDriverCode( void ){
	uint16_t data;
	data = ili9325_readReg( ILI9325_REG_DriverCodeRead );
	return data;
}

/**
 * \brief hard reset
 */
void ili9325_reset( void ){
	ili9325_pin_RST_setValue( 1 );
	ili9325_delay_ms( 1 );
	ili9325_pin_RST_setValue( 0 );
	ili9325_delay_ms( 10 );
	ili9325_pin_RST_setValue( 1 );
	ili9325_delay_ms( 50 );
}

/**
 * \brief Initialize this device
 */
void ili9325_init( void ){
	ili9325_pin_setup();

	ili9325_width = 240;
	ili9325_height = 320;

	/* Reset LCD driver */
	ili9325_reset();	

	ili9325_writeReg(0x0001,0x0100); 
	ili9325_writeReg(0x0002,0x0700); 
	//ili9325_writeReg(0x0003,0x10b0);// or 1030
	ili9325_writeReg( 0x03, 0x1030 );
	ili9325_writeReg(0x0004,0x0000); 
	ili9325_writeReg(0x0008,0x0207);  
	ili9325_writeReg(0x0009,0x0000);
	ili9325_writeReg(0x000A,0x0000); 
	ili9325_writeReg(0x000C,0x0000); 
	ili9325_writeReg(0x000D,0x0000);
	ili9325_writeReg(0x000F,0x0000);
	//power on sequence VGHVGL
	ili9325_writeReg(0x0010,0x0000);   
	ili9325_writeReg(0x0011,0x0007);  
	ili9325_writeReg(0x0012,0x0000);  
	ili9325_writeReg(0x0013,0x0000); 
	//vgh 
	ili9325_writeReg(0x0010,0x1290);   
	ili9325_writeReg(0x0011,0x0227);
	//delayms(100);
	//vregiout 
	ili9325_writeReg(0x0012,0x001d); //0x001b
	//delayms(100); 
	//vom amplitude
	ili9325_writeReg(0x0013,0x1500);
	//delayms(100); 
	//vom H
	ili9325_writeReg(0x0029,0x0018); 
	ili9325_writeReg(0x002B,0x000D); 

	//gamma
	ili9325_writeReg(0x0030,0x0004);
	ili9325_writeReg(0x0031,0x0307);
	ili9325_writeReg(0x0032,0x0002);// 0006
	ili9325_writeReg(0x0035,0x0206);
	ili9325_writeReg(0x0036,0x0408);
	ili9325_writeReg(0x0037,0x0507); 
	ili9325_writeReg(0x0038,0x0204);//0200
	ili9325_writeReg(0x0039,0x0707); 
	ili9325_writeReg(0x003C,0x0405);// 0504
	ili9325_writeReg(0x003D,0x0F02); 
	//ram
	ili9325_writeReg(0x0050,0x0000); 
	ili9325_writeReg(0x0051,0x00EF);
	ili9325_writeReg(0x0052,0x0000); 
	ili9325_writeReg(0x0053,0x013F);  
	ili9325_writeReg(0x0060,0xA700); 
	ili9325_writeReg(0x0061,0x0001); 
	ili9325_writeReg(0x006A,0x0000); 
	//
	ili9325_writeReg(0x0080,0x0000); 
	ili9325_writeReg(0x0081,0x0000); 
	ili9325_writeReg(0x0082,0x0000); 
	ili9325_writeReg(0x0083,0x0000); 
	ili9325_writeReg(0x0084,0x0000); 
	ili9325_writeReg(0x0085,0x0000); 
	//
	ili9325_writeReg(0x0090,0x0010); 
	ili9325_writeReg(0x0092,0x0600); 
	ili9325_writeReg(0x0093,0x0003); 
	ili9325_writeReg(0x0095,0x0110); 
	ili9325_writeReg(0x0097,0x0000); 
	ili9325_writeReg(0x0098,0x0000);
	ili9325_writeReg(0x0007,0x0133);
}

void ili9325_exitSleep( void ){
	/* Power On sequence */
	ili9325_writeReg(0x0010, 0x0080); // SAP, BT[3:0], AP, DSTB, SLP 
	ili9325_writeReg(0x0011, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0] 
	ili9325_writeReg(0x0012, 0x0000); // VREG1OUT voltage 
	ili9325_writeReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude 
	//delayms(200); // Dis-charge capacitor power voltage 
	ili9325_delay_ms( 200 );
	ili9325_writeReg(0x0010, 0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB 
	ili9325_writeReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0] 
	//delayms(50); // delay 50ms 
	ili9325_delay_ms( 50 );
	ili9325_writeReg(0x0012, 0x001C); //Inernal reference voltage =Vci; 
	//delayms(50); // delay 50ms 
	ili9325_delay_ms( 50 );
	ili9325_writeReg(0x0013, 0x1A00); // VDV[4:0] for VCOM amplitude 
	ili9325_writeReg(0x0029, 0x0025); // VCM[5:0] for VCOMH 
	//delayms(50); // delay 50ms 
	ili9325_delay_ms( 50 );
	ili9325_writeReg(0x0007, 0x0133); // 262K color and display ON 
}

void ili9325_enterSleep( void ){
	ili9325_writeReg(0x0007, 0x0131); // Set D1=0, D0=1 
	//delayms(10); 
	ili9325_delay_ms( 10 );
	ili9325_writeReg(0x0007, 0x0130); // Set D1=0, D0=0 
	//delayms(10); 
	ili9325_delay_ms( 10 );
	ili9325_writeReg(0x0007, 0x0000); // display OFF 
	/* Power OFF sequence */
	ili9325_writeReg(0x0010, 0x0080); // SAP, BT[3:0], APE, AP, DSTB, SLP 
	ili9325_writeReg(0x0011, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0] 
	ili9325_writeReg(0x0012, 0x0000); // VREG1OUT voltage ili9325_writeReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude delayms(200); // Dis-charge capacitor power voltage 
	ili9325_writeReg(0x0010, 0x0082); // SAP, BT[3:0], APE, AP, DSTB, SLP
}

/**
 * \brief Set GRAM address
 */
void ili9325_setCurrentPos( uint16_t x, uint16_t y ){
	ili9325_writeReg( ILI9325_REG_HorizontalGRAMAddrSet, x );
	ili9325_writeReg( ILI9325_REG_VerticalGRAMAddrSet, y );
}

void ili9325_setWindowArea( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end ){
	ili9325_width = x_end - x_start + 1;
	ili9325_height = y_end - y_start + 1;


	ili9325_writeReg( ILI9325_REG_HorizontalAddrStart, x_start );
	ili9325_writeReg( ILI9325_REG_HorizontalAddrEnd, x_end );
	ili9325_writeReg( ILI9325_REG_VerticalAddrStart, y_start );
	ili9325_writeReg( ILI9325_REG_VerticalAddrEnd, y_end );
	ili9325_setCurrentPos( x_start, y_start );

}

void ili9325_clearScreen( uint16_t color ){
	ili9325_setWindowArea( 0,0, ILI9325_WIDTH-1, ILI9325_HEIGHT-1 );
	ili9325_fillWindow( color );
}

void ili9325_fillWindow( uint16_t color ){
	uint16_t h,w;
	ili9325_selectReg( 0x22 );
	for( h=0; h<ili9325_height; ++h ){
		for( w=0; w<ili9325_width; ++w ){
			ili9325_writeData( color );
		}
	}
}

