#include <stdio.h>
#include <ioport.h>
#include <stdint.h>
#include <interrupt.h>
#include <util/delay.h>

#include "xpt2046.h"

/* Global variable*/
static xpt2046_pen_irq_handler_t irq_handler = NULL;
uint16_t __xpt2046_screen_width;
uint16_t __xpt2046_screen_height;


uint8_t __xpt2046_rwByte( uint8_t data ){
	uint8_t idx;
	/*
	 * DCLK high: min 200 ns
	 * DCLK low: min 200 ns
	 * DIN valid prior to DCLK rasing: 100 ns
	 * DIN hold after DCLK high
	 * DCLK falling to DOUT valid: max 200 ns
	 */

	for( idx=0; idx<8; ++idx ){
		xpt2046_pin_DIN_setValue( data&0x80 );
		/* delay 100 ns */ 
		xpt2046_pin_DCLK_setValue( 1 );
		/* delay 200 ns */
		data <<= 1;
		data |= xpt2046_pin_DOUT_getValue();
		xpt2046_pin_DCLK_setValue( 0 );
		/* delay 200 ns */
	}
	xpt2046_pin_DIN_setValue( 0 );
	return data;
}

/**
 * A complete conversion
 */
uint16_t __xpt2046_conversion( uint8_t type ){
	uint16_t adt_data;
	/*
	 * CS falling to first DCLK raising: min 100 ns
	 */
	xpt2046_pin_CS_setValue( 0 );
	/* delay 100 ns */
	__xpt2046_rwByte( 0x80 | type );

	xpt2046_pin_DCLK_setValue( 0 );
	xpt2046_pin_DCLK_setValue( 1 );

	adt_data = __xpt2046_rwByte( 0 );
	adt_data <<= 4;
	adt_data += __xpt2046_rwByte( 0 );

	xpt2046_pin_CS_setValue( 1 );

	return adt_data;
}


void xpt2046_init( void ){

	xpt2046_pin_setup();
	/* set extern interrupt */
	eint_enable( XPT_PIN_PENIRQ, EINT_TRIG_MODE_FALLING );
}


void __sort( uint16_t *arr, uint8_t n ){
	uint8_t i, j;
	uint16_t temp;
	uint8_t idx;

	for( i=0; i<n; ++i ){
		idx = i;
		temp = arr[i];
		for( j=i+1; j<n; ++j ){
			if( arr[j] < temp ){
				idx = j;
				temp = arr[j];
			}
		}
		arr[idx] = arr[i];
		arr[i] = temp;

	}
}
void xpt2046_getTouchPos( xpt2046_position_t *data ){
	#define SN 6
	uint16_t x[SN], y[SN];
	uint8_t idx;

	for( idx=0; idx<SN; ++idx ){
		x[idx] = __xpt2046_conversion( XPT2046_DFR_X );
		y[idx] = __xpt2046_conversion( XPT2046_DFR_Y );
	}

	__sort( x, SN );
	__sort( y, SN );

	data->x = 0;
	data->y = 0;
	for( idx=1; idx<SN-1; ++idx ){
		data->x += x[idx];
		data->y += y[idx];
	}
	data->x /= SN-2;
	data->y /= SN-2;
}

void xpt2046_penIrqRegister( xpt2046_pen_irq_handler_t handler ){
	irq_handler = handler;
}

void xpt2046_setScreenSize( uint16_t width, uint16_t heigh ){
	__xpt2046_screen_width = width;
	__xpt2046_screen_height = heigh;
}

/* Interrupt handler */
EINT_ISR( XPT_PIN_PENIRQ ){
	//cli();
	eint_disable( XPT_PIN_PENIRQ );	
	if( irq_handler )
		irq_handler();

	eint_clearIrqFlag( XPT_PIN_PENIRQ );
	eint_enable( XPT_PIN_PENIRQ, EINT_TRIG_MODE_FALLING );
}


