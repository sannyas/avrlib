/**
 * @file timer0.h
 */

#ifndef _H_TIMER0_
#define _H_TIMER0_

/**
 * @addtogroup grp_avr AVR
 * @{
 */

/**
 * @addtogroup grp_timer0 timer0
 * @{
 */
#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

/** 
 * @brief Waveform generation mode 
 * @{
 */
#define TIMER0_WGM_NORMAL	0x00
#define TIMER0_WGM_CORRENT_PWM	0x40
#define TIMER0_WGM_CTC		0x08
#define TIMER0_WGM_FAST_PWM	0x48
/** @} */

/**
 * @brief Compare match output mode
 * @{
 */
#define TIMER0_COM_NORMAL	0x00
#define TIMER0_COM_TOGGLE_OC0	0x10
#define TIMER0_COM_CLEAR_OC0	0x20
#define TIMER0_COM_SET_OC0	0x30
/** @} */

/**
 * @brief Colock select.
 * @{
 */
#define TIMER0_CLOCK_NONE	0x00
#define TIMER0_CLOCK_DIV1	0x01
#define TIMER0_CLOCK_DIV8	0x02
#define TIMER0_CLOCK_DIV64	0x03
#define TIMER0_CLOCK_DIV256	0x04
#define TIMER0_CLOCK_DIV1024	0x05
#define TIMER0_CLOCK_T0_RASING	0x06
#define TIMER0_CLOCK_T0_FALLING	0x07
/** @} */


#define PWM_MODE_FAST		TIMER0_WGM_FAST_PWM \
				| TIMER0_COM_CLEAR_OC0 
#define PWM_MODE_CORRECT	TIMER0_WGM_CORRENT_PWM \
				| TIMER0_COM_CLEAR_OC0
#define PWM_FREQUENCY_MAX	F_CPU
#define PWM_FREQUENCY(n) \
	(((F_CPU+freq/2)/freq)>1024)? 102ik4
	
#define timer0_pwm_init( mode,freq ) \
	TCCR0 = mode | PWM_FREQUENCY(freq);




#endif /* __AVR_* */

/** @} */
/** @} */

#endif
