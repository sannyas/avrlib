#include <enc28j60.h>

#ifdef ETHERNET_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "ixrip.h"

void ethernet_send( uint16_t len ){
	DEBUG_FUNC_ENTER();

	/* paint the data to be sent.
	uint8_t i,j;
	for( i=0; i<10; ++i ){
		for( j=0; j<8; ++j ){
			printf( "%x, ", buff.bytes[i*8+j] );
		}
		printf( "\n" );
	}
	*/

	/* TODO */
	enc28j60_transmitPacket( buff.bytes, len, 0 );

	DEBUG_FUNC_EXIT();
}

void ethernet_handle( void ){
	DEBUG_FUNC_ENTER();	

	if( buff.ethernet.len_or_type == ETHERNET_TYPE_ARP ){
		arp_handle( );
	}
	else if( buff.ethernet.len_or_type == ETHERNET_TYPE_IP ){
		ip_handle();
	}
	// else if( 
	
	DEBUG_FUNC_EXIT();
}
