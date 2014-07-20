#ifndef _BOARDLED_H_
#define _BOARDLED_H_

#include <avr/io.h>

void boardled_init( void );
void boardled_on( void );
void boardled_off( void );
void boardled_ch( void );

#endif
