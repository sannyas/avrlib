#include <interrupt.h>
#include <util/delay.h>
#include "xpt2046.h"

#define XPT_CROSS_SIZE 5

static float __fac_x;
static uint16_t __con_x;
static float __fac_y;
static uint16_t __con_y;

static void __xpt2046_drawCross( xpt2046_drawPixel_t draw, uint16_t x, uint16_t y ){
	uint8_t idx;
	for( idx=0; idx<XPT_CROSS_SIZE; ++idx ){
		draw( x+idx, y+idx );
		draw( x-idx, y-idx );
		draw( x+idx, y-idx );
		draw( x-idx, y+idx );
	}
}

/* 两点 */
void xpt2046_startAdjust( xpt2046_drawPixel_t draw ){
	uint8_t idx;

	xpt2046_position_t touch_pos[2];
	#define touch_pos1 touch_pos[0]
	#define touch_pos2 touch_pos[1]

	uint16_t screen_pos[4];
	#define screen_x1 screen_pos[0]
	#define screen_y1 screen_pos[1]
	#define screen_x2 screen_pos[2]
	#define screen_y2 screen_pos[3]

	screen_x1 = __xpt2046_screen_width/8;
	screen_y1 = __xpt2046_screen_height/8;
	screen_x2 = screen_x1 * 7 - 1;
	screen_y2 = screen_y1 * 7 - 1;
	screen_x1 -= 1;
	screen_y1 -= 1;

	/* 禁止PenIrq中断 */
	eint_disable( XPT_PIN_PENIRQ );

	for( idx=0; idx<2; idx++ ){

		/* 等待触摸动作释放 */
		while( xpt2046_pin_PENIRQ_getValue() == 0 );
		xpt2046_delay_ms( 100 );

		__xpt2046_drawCross( draw, screen_pos[idx*2], screen_pos[idx*2+1]);

		/* 等待按下 */
		while( xpt2046_pin_PENIRQ_getValue() == 1 );
		xpt2046_delay_ms( 10 );

		xpt2046_getTouchPos( touch_pos+idx );
	}
	
	__fac_x = ((float)screen_x1-screen_x2) /
		((float)(touch_pos1.x)-touch_pos2.x);
	__fac_y = ((float)screen_y1-screen_y2) /
		((float)(touch_pos1.y)-touch_pos2.y);
	__con_x = screen_x1 - (uint16_t)(__fac_x*touch_pos1.x);
	__con_y = screen_y1 - (uint16_t)(__fac_y*touch_pos1.y);


	eint_clearIrqFlag( XPT_PIN_PENIRQ );
	eint_enable( XPT_PIN_PENIRQ, EINT_TRIG_MODE_FALLING );
}

void xpt2046_getScreenPos( xpt2046_position_t *pos ){
	pos->x = __fac_x*pos->x + __con_x;
	pos->y = __fac_y*pos->y + __con_y;
}
