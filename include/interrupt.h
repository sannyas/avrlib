#include <avr/interrupt.h>
#include <ioport.h>

/* for device: atmega16 */
#if DEVICE == atmega16 || DEVICE == m16

#define INT_IDX_EINT0	0
#define INT_IDX_EINT1	1
#define INT_IDX_EINT2	2

#define MAP_PIN_TO_IDX_D2	INT_IDX_EINT0
#define MAP_PIN_TO_IDX_D3	INT_IDX_EINT1
#define MAP_PIN_TO_IDX_D4	INT_IDX_EINT2

#define MAP_PIN_TO_ISR_D2 	INT0_vect
#define MAP_PIN_TO_ISR_D3 	INT1_vect
#define MAP_PIN_TO_ISR_D4 	INT2_vect

#define MAP_PIN_TO_INT_BIT_D2 	INT0
#define MAP_PIN_TO_INT_BIT_D3 	INT1
#define MAP_PIN_TO_INT_BIT_D4 	INT2

#define MAP_PIN_TO_INTF_BIT_D2 	INTF0
#define MAP_PIN_TO_INTF_BIT_D3 	INTF1
#define MAP_PIN_TO_INTF_BIT_D4 	INTF2

#define EINT_TRIG_MODE_LOW	0
#define EINT_TRIG_MODE_CHANGE	1
#define EINT_TRIG_MODE_FALLING	2
#define EINT_TRIG_MODE_RISING	3

extern inline void __i_eint_enable( uint8_t idx, uint8_t mode ){
	if( idx == INT_IDX_EINT0 ){
		GICR |= _BV(INT0);
		MCUCR &= ~(0x03);
		MCUCR |= mode;
		
	}
	else if( idx == INT_IDX_EINT1 ){
		GICR |= _BV(INT1);
		MCUCR &= ~(0x03<<2);
		MCUCR |= mode<<2;
	}
	else if( idx == INT_IDX_EINT2 ){
		GICR |= _BV(INT2);
		if( mode == EINT_TRIG_MODE_RISING ){
			MCUCSR |= _BV(ISC2);
		}
		else if( mode == EINT_TRIG_MODE_FALLING ){
			MCUCSR &= ~_BV(ISC2);
		}
	}
}

extern inline void __i_eint_disable(uint8_t idx){
	if( idx == INT_IDX_EINT0 )
		GICR &= ~_BV(INT0);
	else if( idx == INT_IDX_EINT1 )
		GICR &= ~_BV(INT1);
	else if( idx == INT_IDX_EINT2 )
		GICR &= ~_BV(INT2);
}

#define __eint_enable(p,trig_mode) \
	__i_eint_enable( MAP_PIN_TO_IDX_##p, trig_mode )
#define eint_enable(pin,trig_mode) __eint_enable(pin,trig_mode)
#define __eint_disable(pin) \
	__i_eint_disable( MAP_PIN_TO_IDX_##pin )
#define eint_disable(pin) __eint_disable(pin)
	
#define __eint_getIrqFlag(pin) \
	( GIFR&_BV(MAP_PIN_TO_INTF_BIT_D2_##pin ) )
#define eint_getIrqFlag(pin) \
	__eint_getIrqFlag( pin )

#define __eint_clearIrqFlag(pin) \
	( GIFR&=_BV(MAP_PIN_TO_INTF_BIT_##pin ) )
#define eint_clearIrqFlag(pin) \
	__eint_clearIrqFlag( pin )


#define __EINT_ISR(pin) ISR(MAP_PIN_TO_ISR_##pin)
#define EINT_ISR(pin) __EINT_ISR(pin)

#endif /* DEVICE == atmega16 || DEVICE == m16 */
