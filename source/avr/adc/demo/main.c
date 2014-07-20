#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "../adc.h"
#include <usart.h>

int main( void ){
	uint8_t idx;
	uint16_t result;

	DDRA = 0;

	usart_init();
	stdout = &usart_file;

	
	adc_config( ADC_PRESCALE_AUTO
			| ADC_INTERRUPT_DISABLE
			| ADC_AUTO_TRIG_DISABLE 
		  );
	adc_setVRef( ADC_REF_AVCC );
	adc_setChannel( ADC_CHANNEL_ADC1 );
	adc_enable();
	
	/* 
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
	ADMUX |= (1<<REFS0);
	ADCSRA |= 1<<ADEN;
	*/


	//printf( "20 times single conversion:\n" );
	//for( idx=0; idx<20; ++idx ){
	while( 1 ){

		for( idx=0; idx<8; ++idx ){
			adc_setChannel( ADC_CHANNEL_FROM_IDX(idx) );
			adc_startConversion( );
			adc_waitConversion( );
			printf( "ADC%d = %d\n", idx, ADC );
		}


		printf( "\n" );

		/*
		ADCSRA |= (1 << ADSC); 
		while(!(ADCSRA & (1 << ADIF))); 
		ADCSRA |= (1 << ADIF); 
		printf( "ADC0 = %d\n", ADC );
		*/
		_delay_ms( 1000 );
	}

	return 0;
}
