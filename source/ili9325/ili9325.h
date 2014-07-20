#ifndef __ILI9325_H__
#define __ILI9325_H__
#include <stdint.h>

/** 
 * @name registers
 * Register descriptions */
/*@{*/
#define ILI9325_REG_Index
#define ILI9325_REG_StatusRead
#define ILI9325_REG_DriverCodeRead		0x00
#define ILI9325_REG_DriverOutputCtrl1		0x01
#define ILI9325_REG_LCDDrivingCtrl		0x02
#define ILI9325_REG_EntryMode			0x03
#define ILI9325_REG_ResizeCtrl			0x04
#define ILI9325_REG_DisplayCtrl1		0x07
#define ILI9325_REG_DisplayCtrl2		0x08
#define ILI9325_REG_DisplayCtrl3		0x09
#define ILI9325_REG_DisplayCtrl4		0x0a
#define ILI9325_REG_RGBDisplayInterfaceCtrl1	0x0c
#define ILI9325_REG_FrameMakerPos		0x0c
#define ILI9325_REG_RGBDisplayInterfaceCtrl2	0x0f
#define ILI9325_REG_PowerCtrl1			0x10
#define ILI9325_REG_PowerCtrl2			0x11
#define ILI9325_REG_PowerCtrl3			0x12
#define ILI9325_REG_PowerCtrl4			0x13
#define ILI9325_REG_HorizontalGRAMAddrSet	0x20
#define ILI9325_REG_VerticalGRAMAddrSet		0x21
#define ILI9325_REG_WriteDataToGRAM		0x22
#define ILI9325_REG_PowerCtrl7			0x29
#define ILI9325_REG_FrameRateAndColorCtrl	0x2b
#define ILI9325_REG_GammaCtrl1			0x30
#define ILI9325_REG_GammaCtrl2			0x31
#define ILI9325_REG_GammaCtrl3			0x32
#define ILI9325_REG_GammaCtrl4			0x35
#define ILI9325_REG_GammaCtrl5			0x36
#define ILI9325_REG_GammaCtrl6			0x37
#define ILI9325_REG_GammaCtrl7			0x38
#define ILI9325_REG_GammaCtrl8			0x39
#define ILI9325_REG_GammaCtrl9			0x3c
#define ILI9325_REG_GammaCtrl10			0x3d
#define ILI9325_REG_HorizontalAddrStart		0x50
#define ILI9325_REG_HorizontalAddrEnd		0x51
#define ILI9325_REG_VerticalAddrStart		0x52
#define ILI9325_REG_VerticalAddrEnd		0x53
#define ILI9325_REG_DriverOutputCtrl2		0x60
#define ILI9325_REG_BaseImaeDisplayCtrl		0x61
#define ILI9325_REG_VerticalScrollControl	0x6a
#define ILI9325_REG_PartialImg1DispalyPos	0x80
#define ILI9325_REG_PartialImg1AreaStartLine	0x81
#define ILI9325_REG_PartialImg1AreaEndLine	0x82
#define ILI9325_REG_PartialImg2DispalyPos	0x83
#define ILI9325_REG_PartialImg2AreaStartLine	0x84
#define ILI9325_REG_PartialImg2AreaEndLine	0x85
#define ILI9325_REG_PanelInterfaceCtrl1		0x90
#define ILI9325_REG_PanelInterfaceCtrl2		0x92
#define ILI9325_REG_PanelInterfaceCtrl4		0x95
#define ILI9325_REG_OTPVCMProgCtrl		0xa1
#define ILI9325_REG_OTPVCMStatusAndEnable	0xa2
#define ILI9325_REG_OTPProgIDKey		0xa5
/*@}*/

/** 
 * @name Some color 
 */

/*@{*/
#define ILI9325_WHITE         	 0xFFFF
#define ILI9325_BLACK         	 0x0000	  
#define ILI9325_BLUE         	 0x001F  
#define ILI9325_BRED             0XF81F
#define ILI9325_GRED		 0XFFE0
#define ILI9325_GBLUE		 0X07FF
#define ILI9325_RED           	 0xF800
#define ILI9325_MAGENTA       	 0xF81F
#define ILI9325_GREEN         	 0x07E0
#define ILI9325_CYAN          	 0x7FFF
#define ILI9325_YELLOW        	 0xFFE0
#define ILI9325_BROWN 		 0XBC40
#define ILI9325_BRRED 		 0XFC07
#define ILI9325_GRAY  		 0X8430
/*@}*/

/**
 * @brief window area struct
 *
 * This struct define a window area in screen.
 */
typedef struct ili9325_WindowArea{
	uint16_t h_start; /**< horizontal start */
	uint16_t h_end;   /**< horizontal end */
	uint16_t v_start; /**< vertical start  */
	uint16_t v_end;   /**< vertical end */
} ili9325_window_area_t;

/**
 * @name low-level operations
 * low-level operations 
 *
 * @{
 */
void ili9325_selectReg( uint16_t reg );
void ili9325_writeData( uint16_t data );
void ili9325_writeReg( uint8_t idx, uint16_t data );
uint16_t ili9325_readReg( uint8_t idx );
/*@}*/

/** 
 * @name Device init and power manage 
 *
 * @{
 */
uint16_t ili9325_getDriverCode( void );
void ili9325_reset( void );
void ili9325_init( void );
void ili9325_exitSleep( void );
void ili9325_enterSleep( void );
/*@}*/

/**
 * @name Paint control
 *
 * @{
 */
void ili9325_setCurrentPos( uint16_t x, uint16_t y );
void ili9325_setWindowArea( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end );
void ili9325_fillWindow( uint16_t color );
/*@}*/

/* Paint operations */
/* 
void ili9325_clearScreen( uint16_t clolr );
void ili9325_drawPoint( uint16_t x, uint16_t y, uint16_t color, uint16_t size );
void ili9325_drawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, uint16_t width );
void ili9325_drawRect( uint16_t x_tl, uint16_t y_tl, uint16_t x_br, uint16_t y_br, uint16_t color, uint16_t width );
void ili9325_fillRect( uint16_t x_tl, uint16_t y_tl, uint16_t x_br, uint16_t y_br, uint16_t color );
void ili9325_drawCicle( uint16_t x, uint16_t y, uint16_t r, uint16_t color, uint16_t width );
*/

/* pins operation */
extern void ili9325_pin_setup();
extern void ili9325_pin_RS_setValue( uint8_t v );
extern void ili9325_pin_CS_setValue( uint8_t v );
extern void ili9325_pin_WR_setValue( uint8_t v );
extern void ili9325_pin_RD_setValue( uint8_t v );
extern void ili9325_pin_RST_setValue( uint8_t v );
extern void ili9325_pin_DB_setValue( uint8_t v ); 
extern void ili9325_pin_DB_setOutMode( void );
extern void ili9325_pin_DB_setInMode( void );
extern uint8_t ili9325_pin_DB_getValue( void );

#endif
