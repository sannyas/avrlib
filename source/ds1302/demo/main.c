#include <stdio.h>
#include <avr/io.h>
#include <usart.h>
#include <ioport.h>
#include <ds1302.h>
#include <util/delay.h>

uint8_t func_call_chain_cnt = 0;

#define NEW_YEAR 14
#define NEW_MONTH 3
#define NEW_DATE 12
#define NEW_DAY 3
#define NEW_HOUR 15
#define NEW_MINUTE 42
#define NEW_SECOND 20

int main( void ){
	ds1302_date_time_t dt;
	ds1302_time_t time;
	ds1302_date_t date;

	usart_init();
	stdout = &usart_file;

	ds1302_init();

	printf( "main start\n" );

	/*  test Date */
	printf( "Test Reading and Write Date\n" );
	printf( "--Geting Date...\n" );
	ds1302_getDate( &date );
	printf( "--The Date is : %d:%d:%d:%d\n",
			date.year,
			date.month,
			date.date,
			date.day );
	printf( "--Set Date to 14/3/12/3 ...\n" );
	date.year = NEW_YEAR;
	date.month = NEW_MONTH;
	date.date = NEW_DATE;
	date.day = NEW_DAY;
	ds1302_setDate( &date );
	printf( "--Reread the Date...\n" );
	ds1302_getDate( &date );
	printf( "--The Date is : %d:%d:%d:%d\n",
			date.year,
			date.month,
			date.date,
			date.day );
	printf( "\n\n" );


	printf( "Test reading and writing Time\n" );
	printf( "--Getting Time...\n" );
	ds1302_getTime( &time );
	printf( "--The time is %d:%d:%d\n", 
			time.hour,
			time.minute,
			time.second );
	printf( "--Seting time to %d:%d:%d...\n", NEW_HOUR, NEW_MINUTE, NEW_SECOND );
	time.hour = NEW_HOUR;
	time.minute = NEW_MINUTE;
	time.second = NEW_SECOND;
	ds1302_setTime( &time );
	printf( "--Reget the Time ...\n" );
	ds1302_getTime( &time );
	printf( "--The time is %d:%d:%d\n", 
			time.hour,
			time.minute,
			time.second );
	printf( "\n\n" );

	while( 1 ){
		ds1302_getDateTime( &dt );
		printf( "\r%d/%d/%d  %d:%d:%d        ",
				dt.year,
				dt.month,
				dt.date,
				dt.hour,
				dt.minute,
				dt.second );
		_delay_ms( 200 );
	}

	return 0;
}
