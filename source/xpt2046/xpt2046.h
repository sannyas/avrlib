#ifndef __XPT2046_H__
#define __XPT2046_H__

#include <util/delay.h>

#define xpt2046_delay_ms(n) _delay_ms(n);
extern uint16_t __xpt2046_screen_width;
extern uint16_t __xpt2046_screen_height;

//Single-Ended Reference
#define XPT2046_SER_TEMP0	0x04
#define XPT2046_SER_Y		0x14
#define XPT2046_SER_BAT		0x24
#define XPT2046_SER_ZX		0x34
#define XPT2046_SER_ZY		0x44
#define XPT2046_SER_X		0x54
#define XPT2046_SER_AUX		0x64
#define XPT2046_SER_TEMP1	0x74

// Differential Reference
#define XPT2046_DFR_Y		0x10
#define XPT2046_DFR_ZX		0x30
#define XPT2046_DFR_ZY		0x40
#define XPT2046_DFR_X		0x50

#define XPT2046_MODE_12BIT	0x00 #define XPT2046_MODE_8BIT	0x08


#define REFERENCE_ON	0x02
#define REFERENCE_OFF	0x00

#define ADC_ON		0x01
#define ADC_OFF		0x00

/**
 * Point struct
 */
typedef struct xpt2046_Position{
	uint16_t x;
	uint16_t y;
} xpt2046_position_t;

typedef void (*xpt2046_pen_irq_handler_t)(void);

void xpt2046_init( void );
void xpt2046_penIrqRegister( xpt2046_pen_irq_handler_t irq_handler );
void xpt2046_getTouchPos( xpt2046_position_t *pos );
void xpt2046_setScreenSize( uint16_t width, uint16_t height );

/************  Adjusting **************/

typedef void (*xpt2046_drawPixel_t)(uint16_t x, uint16_t y);

void xpt2046_startAdjust( xpt2046_drawPixel_t draw );
void xpt2046_getScreenPos( xpt2046_position_t *pos );

/*********** Pins operations ****************/
extern void xpt2046_pin_setup( void );
extern void xpt2046_pin_CS_setValue( uint8_t v );
extern void xpt2046_pin_DCLK_setValue( uint8_t v );
extern void xpt2046_pin_DIN_setValue( uint8_t v );
extern uint8_t xpt2046_pin_DOUT_getValue( void );
extern uint8_t xpt2046_pin_PENIRQ_getValue( void );

#endif 
