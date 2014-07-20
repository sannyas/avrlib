#include <stdio.h>
#include <util/delay.h>

#ifdef DS1302_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "ds1302.h"

static uint8_t __bcd_to_decimal( uint8_t data ){
	return (data>>4)*10 + (data&0x0f);
}

static uint8_t __decimal_to_bcd( uint8_t data ){
	return ((data/10)<<4)+data%10;
}

static void __ds1302_writeByte( uint8_t data ){
	uint8_t idx;

	ds1302_pin_IO_setOutMode();
	//DDRD &= ~_BV(D7);
	_delay_ms( 1 );

	for( idx=0; idx<8; ++idx ){
		ds1302_pin_IO_set( data&0x01 );
		_delay_ms( 1 );

		ds1302_pin_SCLK_set( 1 );
		/* Date to CLK Setup: 200 ns */
		_delay_ms( 1 );

		ds1302_pin_SCLK_set( 0 );
		/* CLK to Date Hold: 280 ns */
		_delay_ms( 1 );

		data >>= 1;
	}
}

static uint8_t __ds1302_readByte( void ){
	uint8_t idx;
	uint8_t data = 0;

	DEBUG_FUNC_ENTER();

	ds1302_pin_IO_setInMode();
	//DDRD |= _BV(D7);
	_delay_ms( 1 );

	for( idx=0; idx<8; ++idx ){
		data >>= 1;
		if( ds1302_pin_IO_get() )
			data |= 0x80;
		ds1302_pin_SCLK_set( 1 );
		_delay_ms( 1 );
		ds1302_pin_SCLK_set( 0 );
		_delay_ms( 1 );
	}

	DEBUG_FUNC_EXIT();

	return data;
}

/* init */
void ds1302_init( void ){
	ds1302_pin_setUp();
}

/* Read & write register */
void ds1302_writeReg( uint8_t addr, uint8_t data ){
	ds1302_pin_RST_set( 1 );
	_delay_ms( 1 );

	__ds1302_writeByte( addr );
	__ds1302_writeByte( data );

	ds1302_pin_RST_set( 0 );
	_delay_ms( 1 );
}

uint8_t ds1302_readReg( uint8_t addr ){
	uint8_t data;

	ds1302_pin_RST_set( 1 );
	_delay_ms( 1 );

	__ds1302_writeByte( addr );
	data = __ds1302_readByte( );

	ds1302_pin_RST_set( 0 );
	_delay_ms( 1 );

	return data;
}


/* Get funcitons */
uint8_t __ds1302_getSecond( void ){
	uint8_t s;

	DEBUG_FUNC_ENTER();

	s = ds1302_readReg( DS1302_REG_ADDR_SEC_READ );
	s = __bcd_to_decimal( s );

	DEBUG_FUNC_EXIT();

	return s;
}

uint8_t __ds1302_getMinute( void ){
	uint8_t m;

	m = ds1302_readReg( DS1302_REG_ADDR_MIN_READ );
	m = __bcd_to_decimal( m );

	return m;
}

/* return 00-24 */
void __ds1302_setHour( uint8_t );
uint8_t __ds1302_getHour( void ){
	uint8_t h;
	uint8_t temp;
	
	h = ds1302_readReg( DS1302_REG_ADDR_HR_READ );

	/* if 12, set to 24 */
	if( h & 0x80 ){
		h &= 0x7f;
		if( h & 0x20 ){ // pm
			h &= ~0x20;
			h = __bcd_to_decimal( h );
			h += 12;
			temp = __decimal_to_bcd( h );
		}
		else{	// am
			h = __bcd_to_decimal( h );
			temp = h;
		}
		__ds1302_setHour( temp );
		/* TODO */

	}
	else
		h = __bcd_to_decimal( h );

	return h;
}

/* return 1-7 */
uint8_t __ds1302_getDay( void ){
	uint8_t d;

	d = ds1302_readReg( DS1302_REG_ADDR_DAY_READ );
	return d;
}

uint8_t __ds1302_getDate( void ){
	uint8_t d;

	d = ds1302_readReg( DS1302_REG_ADDR_DATE_READ );
	d = __bcd_to_decimal( d );
	return d;
}

/* return 1-12 */
uint8_t __ds1302_getMonth( void ){
	uint8_t m;

	m = ds1302_readReg( DS1302_REG_ADDR_MONTH_READ );
	m = __bcd_to_decimal( m );
	return m;
}


/* return 00 - 99 */
uint8_t __ds1302_getYear( void ){
	uint8_t y;

	y = ds1302_readReg( DS1302_REG_ADDR_YEAD_READ );
	y = __bcd_to_decimal( y );
	return y ;
}

/* 00 -59 */
void __ds1302_setSecond( uint8_t s ){
	s = __decimal_to_bcd( s );
	ds1302_writeReg( DS1302_REG_ADDR_SEC_WRITE, s );
}

void __ds1302_setMinute( uint8_t m ){
	m = __decimal_to_bcd( m );
	ds1302_writeReg( DS1302_REG_ADDR_MIN_WRITE, m );
}

/* 00 - 23 */
void __ds1302_setHour( uint8_t h ){
	h = __decimal_to_bcd( h );
	ds1302_writeReg( DS1302_REG_ADDR_HR_WRITE, h );
}

/* 1-7 */
void __ds1302_setDay( uint8_t d ){
	ds1302_writeReg( DS1302_REG_ADDR_DAY_WRITE, d );
}

void __ds1302_setDate( uint8_t d ){
	d = __decimal_to_bcd( d );
	ds1302_writeReg( DS1302_REG_ADDR_DATE_WRITE, d );
}

void __ds1302_setMonth( uint8_t m ){
	m = __decimal_to_bcd( m );
	ds1302_writeReg( DS1302_REG_ADDR_MONTH_WRITE, m );
}

void __ds1302_setYear( uint8_t y ){
	y = __decimal_to_bcd( y );
	ds1302_writeReg( DS1302_REG_ADDR_YEAD_WRITE, y );
}



uint8_t ds1302_get( ds1302_clock_component_t t ){
	DEBUG_FUNC_ENTER();

	uint8_t temp = 0;

	switch( t ){
	case DS1302_SECOND:
		temp = __ds1302_getSecond();
		break;
	case DS1302_MINUTE:
		temp = __ds1302_getMinute();
		break;
	case DS1302_HOUR:
		temp = __ds1302_getHour();
		break;
	case DS1302_DAY:
		temp = __ds1302_getDay();
		break;
	case DS1302_DATE:
		temp = __ds1302_getDate();
		break;
	case DS1302_MONTH:
		temp = __ds1302_getMonth();
		break;
	case DS1302_YEAR:
		temp = __ds1302_getYear();
		break;
	}

	DEBUG_FUNC_EXIT();

	return temp;

}

void ds1302_set( ds1302_clock_component_t t, uint8_t d ){
	DEBUG_FUNC_ENTER();

	switch( t ){
	case DS1302_SECOND:
		 __ds1302_setSecond(d);
		break;
	case DS1302_MINUTE:
		__ds1302_setMinute(d);
		break;
	case DS1302_HOUR:
		__ds1302_setHour(d);
		break;
	case DS1302_DAY:
		__ds1302_setDay(d);
		break;
	case DS1302_DATE:
		__ds1302_setDate(d);
		break;
	case DS1302_MONTH:
		__ds1302_setMonth(d);
		break;
	case DS1302_YEAR:
		__ds1302_setYear(d);
		break;
	}


	DEBUG_FUNC_EXIT();
}

/* get & set time */
void ds1302_getTime( ds1302_time_t *time ){
	DEBUG_FUNC_ENTER();

	time->hour = __ds1302_getHour();
	time->minute = __ds1302_getMinute();
	time->second = __ds1302_getSecond();

	DEBUG_FUNC_EXIT();
}

void ds1302_setTime( ds1302_time_t *time ){
	DEBUG_FUNC_ENTER();

	__ds1302_setHour( time->hour );
	__ds1302_setMinute( time->minute );
	__ds1302_setSecond( time->second );

	DEBUG_FUNC_EXIT();
}

/* get & set date */
void ds1302_getDate( ds1302_date_t *date ){
	DEBUG_FUNC_ENTER();

	date->year = __ds1302_getYear();
	date->month = __ds1302_getMonth();
	date->day = __ds1302_getDay();
	date->date = __ds1302_getDate();

	DEBUG_FUNC_EXIT();
}

void ds1302_setDate( ds1302_date_t *date ){
	DEBUG_FUNC_ENTER();

	__ds1302_setYear( date->year );
	__ds1302_setMonth( date->month );
	__ds1302_setDay( date->day );
	__ds1302_setDate( date->date );

	DEBUG_FUNC_EXIT();
}

void ds1302_getDateTime( ds1302_date_time_t *dt ){
	/*
	uint8_t idx;
	uint8_t *data = (uint8_t *)dt;

	ds1302_pin_RST_set( 1 );
	_delay_ms( 1 );

	__ds1302_writeByte( DS1302_REG_ADDR_CLOCK_BURST_READ );
	for( idx=0; idx<8; ++idx ){
		((uint8_t *)dt)[idx] = __ds1302_readByte();
	}
	ds1302_pin_RST_set( 0 );
	_delay_ms( 1 );
	*/
	dt->year = __ds1302_getYear();	
	dt->month = __ds1302_getMonth();
	dt->date = __ds1302_getDate();
	dt->day = __ds1302_getDay();
	dt->hour = __ds1302_getHour();
	dt->minute = __ds1302_getMinute();
	dt->second = __ds1302_getSecond();
}

void ds1302_setDateTime( ds1302_date_time_t *dt ){
	/*
	uint8_t idx;
	uint8_t *data = (uint8_t *)dt;
	ds1302_pin_RST_set( 1 );
	_delay_ms( 1 );

	__ds1302_writeByte( DS1302_REG_ADDR_CLOCK_BURST_WRITE );
	for( idx=0; idx<8; ++idx ){
		__ds1302_writeByte( data[idx] );
	}

	ds1302_pin_RST_set( 0 );
	_delay_ms( 1 );
	*/
	__ds1302_setYear( dt->year );
	__ds1302_setMonth( dt->month );
	__ds1302_setDate( dt->date );
	__ds1302_setDay( dt->day );
	__ds1302_setHour( dt->hour );
	__ds1302_setMinute( dt->minute );
	__ds1302_setSecond( dt->second );
}

void ds1302_setRunning( uint8_t flag ){
	uint8_t temp;

	temp = __ds1302_getSecond();
	if( flag )
		temp &= 0x7f;
	else
		temp |= 0x80;

	__ds1302_setSecond( temp );
}

uint8_t ds1302_isRunning(){
	uint8_t temp;
	temp = __ds1302_getSecond();
	return !(temp&0x80);
}

void ds1302_setWriteProtect( uint8_t flag ){
	if( flag ){
		ds1302_writeReg( DS1302_REG_ADDR_CONTROL_WRITE, 0x80 );
	}
	else{
		ds1302_writeReg( DS1302_REG_ADDR_CONTROL_WRITE, 0x00 );
	}
}

