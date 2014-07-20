#ifdef IP_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "ixrip.h"

#define IP_CHECKSUM_OK	0
#define IP_CHECKSUM_ERROR 1
uint8_t ipHeaderChecksum( void ){
	uint16_t sum;
	sum = checksum( 0, buff.bytes + IP_PACKET_PADDING,
			IP_HEADER_LEN );
	if( sum == 0 )
		return IP_CHECKSUM_OK;
	else
		return IP_CHECKSUM_ERROR;
}


void setIpHeaderChecksum( void ){
	buff.ip.header_checksum = 0;
#if IXRIP_BYTE_ORDER == LIGHT_ENDIAN
	buff.ip.header_checksum = swap_16(
			~checksum( 0,
				buff.bytes + IP_PACKET_PADDING,
				IP_HEADER_LEN )
			);
#elif IXRIP_BYTE_ORDER == BIG_ENDIAN
	buff.ip.header_checksum = ~checksum( 0,
			buff.bytes + IP_PACKET_PADDING,
			IP_HEADER_LEN 
			);
#endif
}

uint8_t sendIPPacket( void ){
	uint8_t *mac_p;
	uint16_t len;

	DEBUG_FUNC_ENTER();

	/* Get destination mac addr */
	mac_p = arp_request( buff.ip.dest_addr );
	if( mac_p == NULL ){
		PDEBUG( "mac not found\n" );
		DEBUG_FUNC_EXIT();
		return IP_SEND_NO_MAC;
	}

	/* FIll ethernet packet */
	macCopy( buff.ethernet.src_addr, ixrip_mac_addr );
	macCopy( buff.ethernet.dest_addr, mac_p );
	buff.ethernet.len_or_type = ETHERNET_TYPE_IP;

	/* Fill ip header */
	buff.ip.ver_and_hlen = 0x45;
	buff.ip.tos = 0;
	// buff.ip.total_len
	// buff.ip.identification
	#if IXRIP_BYTE_ORDER == LIGHT_ENDIAN
	buff.ip.flags_and_fragoff = 0x0040;
	#elif IXRIP_BYTE_ORDER == BIG_ENDIAN
	buff.ip.flags_and_fragoff = 0x4000;
	#endif
	buff.ip.time_to_live = 0x40; /* TODO */
	ipCopy( buff.ip.src_addr, ixrip_ip_addr );
	buff.ip.header_checksum = 0;
	setIpHeaderChecksum();

	len = swap_16( buff.ip.total_len );
	ethernet_send( ETHERNET_HEADER_LEN + len );

	DEBUG_FUNC_EXIT();
	return IP_SEND_OK;
}

void ip_handle( void ){
	uint8_t protocal = buff.ip.protocal;

	DEBUG_FUNC_ENTER();

	if( !ipIsEqual( buff.ip.dest_addr, ixrip_ip_addr ) ){
		PDEBUG( "Not my ip packet\n" );
		DEBUG_FUNC_EXIT();
		return;
	}

	if( protocal == IP_PROTOCAL_ICMP ){
		icmp_handle( );
	}
	else if( protocal == IP_PROTOCAL_UDP ){
		udp_handle( );
	}
	//else if(...
	else{
		PDEBUG( "ignore ip packet.\n" );
	}
	DEBUG_FUNC_EXIT();
}

