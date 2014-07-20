#ifdef ICMP_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "ixrip.h"

void icmp_handle( void ){
	uint8_t type;
	uint8_t len;

	DEBUG_FUNC_ENTER();

	len = swap_16( buff.ip.total_len ) - IP_HEADER_LEN;

	/* answer */
	type = buff.icmp.type;
	if( type == ICMP_TYPE_ECHO_REQUEST ){
		buff.icmp.type = ICMP_TYPE_ECHO_REPLY;
		buff.icmp.code = 0;
		buff.icmp.checksum = 0;
		buff.icmp.checksum =  ~checksum( 0,
					buff.bytes + ICMP_PACKET_OFFSET,
					len );

		ipCopy( buff.ip.dest_addr, buff.ip.src_addr );
		sendIPPacket( );
	}


	DEBUG_FUNC_EXIT();
}
