/**
 * @addtogroup grp_avr AVR
 * @{
 */

/**
 * @addtogroup grp_adc ADC
 * @{
 */

/**
 * @file adc.h
 *
 * This file define some operations for ADT on AVR
 * chip.
 *
 * usage:
 * 	+ Signal Conversion
 * 		adc_config( ADC_PRESCALE_* |
 * 			ADC_INTERRUPT_* |
 * 			ADC_AUTO_TRIG_* );
 * 		adc_setVRefAndChannel( ADC_REF_* | ADC_CHANNEL_* );
 * 		adc_enable( );
 * 		adc_startConversion( );
 * 		adc_waitConversion( );
 * 		uint16_t result = adc_getResult();
 * 	+ Continuout Conversion
 * 		...
 * 
 */
#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>
#include <avr/io.h>

/* ****************************** */
// For ATmega16
/* ****************************** */
#if defined ( __AVR_ATmega16__ ) \
	|| defined( __AVR_ATmega16A__ )

#define ADC_REF_AREF	0
#define ADC_REF_AVCC	(_BV(REFS0))
#define ADC_REF_ONCHIP	(_BV(REFS0)|_BV(REFS1))

#define ADC_ALIGN_LEFT	(_BV(ADLAR))
#define ADC_ALIGN_RIGHT	0

void adc_setVRef( uint8_t ref ){
	ADMUX &= ~(_BV(REFS0)|_BV(REFS1));
	ADMUX |= ref;
}

#define ADC_CHANNEL_ADC0		0x00
#define ADC_CHANNEL_ADC1		0x01
#define ADC_CHANNEL_ADC2		0x02
#define ADC_CHANNEL_ADC3		0x03
#define ADC_CHANNEL_ADC4		0x04
#define ADC_CHANNEL_ADC5		0x05
#define ADC_CHANNEL_ADC6		0x06
#define ADC_CHANNEL_ADC7		0x07
#define ADC_CHANNEL_FROM_IDX(n)		n
#define ADC_CHANNEL_ADC0_ADC0_10X	0x08
#define ADC_CHANNEL_ADC1_ADC0_10X	0x09
#define ADC_CHANNEL_ADC0_ADC0_200X	0x0a
#define ADC_CHANNEL_ADC1_ADC0_200X	0x0b
#define ADC_CHANNEL_ADC2_ADC2_10X	0x0c
#define ADC_CHANNEL_ADC3_ADC2_10X	0x0d
#define ADC_CHANNEL_ADC2_ADC2_200X	0x0e
#define ADC_CHANNEL_ADC3_ADC2_200X	0x0f
#define ADC_CHANNEL_ADC0_ADC1_1X	0x10
#define ADC_CHANNEL_ADC1_ADC1_1X	0x11
#define ADC_CHANNEL_ADC2_ADC1_1X	0x12
#define ADC_CHANNEL_ADC3_ADC1_1X	0x13
#define ADC_CHANNEL_ADC4_ADC1_1X	0x14
#define ADC_CHANNEL_ADC5_ADC1_1X	0x15
#define ADC_CHANNEL_ADC6_ADC1_1X	0x16
#define ADC_CHANNEL_ADC7_ADC1_1X	0x17
#define ADC_CHANNEL_ADC0_ADC2_1X	0x18
#define ADC_CHANNEL_ADC1_ADC2_1X	0x19
#define ADC_CHANNEL_ADC2_ADC2_1X	0x1a
#define ADC_CHANNEL_ADC3_ADC2_1X	0x1b
#define ADC_CHANNEL_ADC4_ADC2_1X	0x1c
#define ADC_CHANNEL_ADC5_ADC2_1X	0x1d
#define ADC_CHANNEL_1D22
#define ADC_CHANNEL_GND

void adc_setChannel( uint8_t ch ){
	ADMUX &= ~0x1f;
	ADMUX |= ch;
}

void adc_setVRefAndChannel( uint8_t cfg ){
	ADMUX = cfg;
}

/* ADCSRA */
#define ADC_PRESCALE_2			0x01
#define ADC_PRESCALE_4			0x02
#define ADC_PRESCALE_8			0x03
#define ADC_PRESCALE_16			0x04
#define ADC_PRESCALE_32			0x05
#define ADC_PRESCALE_64			0x06
#define ADC_PRESCALE_128		0x07

/* The frequency is 50K-200K */
#define _PRES_MIN	(F_CPU/200000)
#if _PRES_MIN < 2
  #define _PRES_AUTO	ADC_PRESCALE_2
#elif _PRES_AUTO < 4
  #define _PRES_AUTO	ADC_PRESCALE_4
#elif _PRES_AUTO < 8
  #define _PRES_AUTO	ADC_PRESCALE_8
#elif _PRES_AUTO < 16
  #define _PRES_AUTO	ADC_PRESCALE_16
#elif _PRES_AUTO < 32	
  #define _PRES_AUTO	ADC_PRESCALE_32
#elif _PRES_AUTO < 64
  #define _PRES_AUTO	ADC_PRESCALE_64
#elif _PRES_AUTO < 128
  #define _PRES_AUTO	ADC_PRESCALE_128
#endif
#define ADC_PRESCALE_AUTO		_PRES_AUTO

#define ADC_INTERRUPT_ENABLE		0x08
#define ADC_INTERRUPT_DISABLE		0x00

#define ADC_AUTO_TRIG_ENABLE		0x20
#define ADC_AUTO_TRIG_DISABLE		0x00

/**
 * @brief ADC Configuration ( ADCSRA )
 *
 * Config control bits in ADCSRA register, including 
 * prescale, interrupt enable or disable and auto trigger
 * enable or disable
 *
 * @param cfg [in] ADC_PRESCALE_* | ADC_INTERRUPT_* | ADC_AUTO_TRIG_* 
 */
extern inline void adc_config( uint8_t cfg ){
	ADCSRA = cfg;
}

extern inline void adc_startConversion( void ){
	ADCSRA |= _BV(ADSC);
}

/* Wait for ADIF to be set and clear ADIF bit */
extern inline void adc_waitConversion( void ){
	while( !(ADCSRA&_BV(ADIF)) );
	ADCSRA |= _BV(ADIF);
}

extern inline void adc_enable( void ){
	ADCSRA |= _BV(ADEN);
}

extern inline void adc_disable( void ){
	ADCSRA &= ~_BV(ADEN);
}

/* Set auto grigger source */
#define ADC_TRIG_SOURCE_CONTINUOUS		(0x00<<5)
#define ADC_TRIG_SOURCE_ANALOG_COMPARATOR	(0x01<<5)
#define ADC_TRIG_SOURCE_EXTERN_INTERRUPT	(0x02<<5)
#define ADC_TRIG_SOURCE_TIMER0_MATCH		(0x03<<5)
#define ADC_TRIG_SOURCE_TIMER0_OVERFLOW		(0x04<<5)
#define ADC_TRIG_SOURCE_TIMER1_MATCH_B		(0x05<<5)
#define ADC_TRIG_SOURCE_TIMER1_OVERFLOW		(0x06<<5)
#define ADC_TRIG_SOURCE_TIMER1_CAPTURE		(0x07<<5)

extern inline void adc_setAutoTriggerSource( uint8_t src ){
	SFIOR &= 0xe0;
	SFIOR |= src;
}

extern inline uint16_t adc_getResult( void ){
	return ADC;
}

#endif /* __AVR_ATmega16A__, __AVR_ATmega16__ */

void adc_doNothing( void );

#endif /* _ADC_H_ */

/** @} */
/** @} */
