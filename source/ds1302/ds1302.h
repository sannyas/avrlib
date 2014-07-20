/**
 * @brief This file define some operation for DS1302 clock module.
 *
 * @author Liao Maoyi
 * @date 2014/03/12
 * usage:
 * 	-# Get or set a component of clock simply.
 * 		If just want to get a component of clock, such as
 * 		one of year, month, date, day, hour, minute and 
 * 		second, you can use ds1302_get and ds1302_set methods
 * 		and call this function with a parameter that specify
 * 		which component you want to get. The sample code:
 *
 * 		...
 * 		uint8_t year; 
 * 		year = ds1302_get( DS1302_YEAR );
 * 		...
 *
 * 	-# Get or set the Time.
 * 		the Time include hour, minute and second. The struct
 * 		ds1302_Time is used to store the time. To get and set
 * 		the Time, you can use ds1302_getTime and ds1302_setTime
 * 		methods, The sample code:
 * 		...
 * 		ds1302_time_t time;
 * 		ds1302_getTime( &time );
 * 		...
 * 	-# Get or set the Date.
 * 		The Date include year, month, date and day( the day of
 * 		the week ). To get and set Date, you can use
 * 		ds1302_getDate and ds1302_setDate method. The sample
 * 		code :
 * 		...
 * 		ds1302_date_t date; 
 * 		ds1302_getDate( &date );
 * 		...
 * 	-# Get or set the DateTime.
 * 		The DateTime include the Date and the Time.Use
 * 		ds1302_getDateTime and ds1302_setDateTime methods
 * 		to get or set the DateTime. the sample code:
 * 		...
 * 		ds1302_date_time_t dt;
 * 		ds1302_getDateTime( &dt );
 * 		...
 */

#ifndef _DS1302_H_
#define _DS1302_H_

#include <stdint.h>


/** 
 * @brief Register address
 *@{
 */

#define DS1302_REG_ADDR_SEC_READ	0x81
#define DS1302_REG_ADDR_SEC_WRITE	0x80
#define DS1302_REG_ADDR_MIN_READ	0x83
#define DS1302_REG_ADDR_MIN_WRITE	0x82
#define DS1302_REG_ADDR_HR_READ		0x85
#define DS1302_REG_ADDR_HR_WRITE	0x84
#define DS1302_REG_ADDR_DATE_READ	0x87
#define DS1302_REG_ADDR_DATE_WRITE	0x86
#define DS1302_REG_ADDR_MONTH_READ	0x89
#define DS1302_REG_ADDR_MONTH_WRITE	0x88
#define DS1302_REG_ADDR_DAY_READ	0x8b
#define DS1302_REG_ADDR_DAY_WRITE	0x8a
#define DS1302_REG_ADDR_YEAD_READ	0x8d
#define DS1302_REG_ADDR_YEAD_WRITE	0x8c
#define DS1302_REG_ADDR_CONTROL_READ	0x8f
#define DS1302_REG_ADDR_CONTROL_WRITE	0x8e
#define DS1302_REG_ADDR_TRICKLE_CHARGE_READ	0x91
#define DS1302_REG_ADDR_TRICKLE_CHARGE_WRITE	0x90
#define DS1302_REG_ADDR_CLOCK_BURST_READ	0xbf
#define DS1302_REG_ADDR_CLOCK_BURST_WRITE	0xbe
/*@}*/

/** 
 * @brief RAM address for reading or writing
 *@{
 */
#define DS1302_RAM_ADDR_READ(n)		(0xc0 + n*2 + 1)
#define DS1302_RAM_ADDR_WRITE(n)	(0xc0 + n*2 )
/*@}*/

/** @brief Date struct */
typedef struct ds1302_Date{
	uint8_t day;	/**< the day of the week */
	uint8_t date;
	uint8_t month;
	uint8_t year;
} ds1302_date_t;

/** @brief Time struct. */
typedef struct ds1302_Time{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
} ds1302_time_t;


/** @brief DateTime struct. */
typedef struct ds1302_DateTime{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;

	uint8_t date;
	uint8_t month;
	uint8_t day;
	uint8_t year;

	uint8_t control;

} ds1302_date_time_t;

/** @brief The component of clock. */
typedef enum ds1302_ClockComponent{
	DS1302_SECOND,
	DS1302_MINUTE,
	DS1302_HOUR,
	DS1302_DAY,
	DS1302_DATE,
	DS1302_MONTH,
	DS1302_YEAR,
} ds1302_clock_component_t;

/**
 * @brief Initialize ds1302.
 */
void ds1302_init( void );

/** Read and write clock register. */
/*@{*/
void	ds1302_writeReg( uint8_t addr, uint8_t data );
uint8_t ds1302_readReg( uint8_t addr );
/*@}*/

/** Read and write ram. */
/*@{*/
void ds1302_writeRam( uint8_t addr, uint8_t data );
uint8_t ds1302_readRam( uint8_t addr );
/*@}*/

/** get and set a component of clock. */
/*@{*/
uint8_t ds1302_get( ds1302_clock_component_t t );
void ds1302_set( ds1302_clock_component_t t, uint8_t d );
/*@}*/

/** get and set time. */
/*@{*/
void ds1302_getTime( ds1302_time_t *time );
void ds1302_setTime( ds1302_time_t *time );
/*@}*/

/** get and set date. */
/*@{*/
void ds1302_getDate( ds1302_date_t *date );
void ds1302_setDate( ds1302_date_t *date );
/*@}*/

/** get and set datetime. */
/*@{*/
void ds1302_getDateTime( ds1302_date_time_t *dt );
void ds1302_setDateTime( ds1302_date_time_t *dt );
/*@}*/

/**
 * @brief Set this chip to run or stop
 *
 * @para if flag is not zero, set it to run, else stop.
 */
void ds1302_setRunning( uint8_t flag );

/**
 * @breif Get running status( stop ot run ).
 *
 * @return If this chip is running, return a nonzero value,
 * 	   else zero.
 */
uint8_t ds1302_isRunning( void );

/**
 * @brief write protect.
 * @para flag If flag is nonzero, the write protet is enable.
 */
void ds1302_setWriteProtect( uint8_t flag );


/*@{*/
/** pin operations. This function should be defined in
 * other file */
extern void ds1302_pin_setUp( void );
extern void ds1302_pin_IO_setOutMode( void );
extern void ds1302_pin_IO_setInMode( void );
extern void ds1302_pin_IO_set( uint8_t );
extern uint8_t ds1302_pin_IO_get( void );
extern void ds1302_pin_RST_set( uint8_t );
extern uint8_t ds1302_pin_RST_get( void );
extern void ds1302_pin_SCLK_set( uint8_t );
extern uint8_t ds1302_pin_SCLK_get( void );
/*@}*/


#endif /* _DS1302_H_ */
