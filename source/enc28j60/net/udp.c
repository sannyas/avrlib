#ifdef UDP_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "ixrip.h"

uint8_t udp_send( uint16_t len ){
	DEBUG_FUNC_ENTER();

	uint16_t udp_check_sum;
	uint8_t ret;

	/* fill ip header */
	buff.ip.total_len = UDP_HEADER_OFFSET + buff.udp.len;
	buff.ip.identification = 0;
	buff.ip.protocal = IP_PROTOCAL_UDP;

	/* fill udp */
	len += 8;
	buff.udp.len = len;
	
	/* Checksum of UDP */
	buff.udp.checksum = 0;
	udp_check_sum = checksum( 0, buff.bytes+UDP_HEADER_OFFSET, len );
	udp_check_sum = checksum( 0, buff.ip.src_addr, 8 );
	udp_check_sum = checksum_add( udp_check_sum, len );
	udp_check_sum = checksum_add( udp_check_sum, IP_PROTOCAL_UDP );
	buff.udp.checksum = udp_check_sum;

	ret = sendIPPacket( );

	DEBUG_FUNC_EXIT();
	return ret;
}

void udp_handle( void ){
	DEBUG_FUNC_ENTER();
	PDEBUG( "src_port %d, dest_port %d\n",
			swap_16( buff.udp.src_port ),
			swap_16( buff.udp.dest_port )
	      );

#ifdef UDP_DEBUG
	uint16_t len = swap_16( buff.udp.len ) - 8;
	uint8_t i;
	if( len > 200 ) len = 200;

	for( i=0; i<len; ++i ){
		PDEBUG_OUTPUT( "%c", buff.udp.data[i] );
	}
	PDEBUG( "\n" );
#endif
	DEBUG_FUNC_EXIT();
}
