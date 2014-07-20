#include <stdio.h>
#ifdef ARP_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "ixrip.h"


void arp_paintCaches( void );

uint8_t *arp_request( uint8_t *ip_addr ){
	DEBUG_FUNC_ENTER();

	uint8_t idx;
	uint8_t *dp;

	dp = arp_findMacInCache( ip_addr );
	if( dp ){
		PDEBUG( "found it, return\n" );
		DEBUG_FUNC_EXIT();
		return dp;
	}

	buff.arp.hardware_type = ARP_HARDWARE_TYPE_ETHERNET;
	buff.arp.protocal_type = ARP_PROTOCAL_TYPE_IP;
	buff.arp.hardware_addr_len = 6;
	buff.arp.protocal_addr_len = 4;
	buff.arp.opcode = ARP_OPCODE_REQUEST;
	buff.ethernet.len_or_type = ETHERNET_TYPE_ARP;

	for( idx=0; idx<4; ++idx ){
		buff.arp.sender_ip_addr[idx] = ixrip_ip_addr[idx];
		buff.arp.receiver_ip_addr[idx] = ip_addr[idx];
	}
	for( idx=0; idx<6; ++idx ){
		buff.arp.sender_hw_addr[idx] = ixrip_mac_addr[idx];
		buff.arp.receiver_hw_addr[idx] = 0x00;
		buff.ethernet.dest_addr[idx] = 0xff;
		buff.ethernet.src_addr[idx] = ixrip_mac_addr[idx];
	}

	ethernet_send( 42 );

	DEBUG_FUNC_EXIT();

	return 0;
}

void arp_answer( void ){
	uint8_t idx;

	DEBUG_FUNC_ENTER();
	/*
	buff.arp.hardware_type = ARP_HARDWARE_TYPE_ETHERNET;
	buff.arp.protocal_type = ARP_PROTOCAL_TYPE_IP;
	buff.arp.hardware_addr_len = 6;
	buff.arp.protocal_addr_len = 4;
	*/
	buff.arp.opcode = ARP_OPCODE_ANSWER;
	for( idx=0; idx<6; ++idx ){
		buff.arp.receiver_hw_addr[idx] =
			buff.arp.sender_hw_addr[idx];
		buff.ethernet.dest_addr[idx] = 
			buff.arp.sender_hw_addr[idx];

		buff.arp.sender_hw_addr[idx] = 
			ixrip_mac_addr[idx];
		buff.ethernet.src_addr[idx] = 
			ixrip_mac_addr[idx];
	}

	for( idx=0; idx<4; ++idx ){
		buff.arp.receiver_ip_addr[idx] = 
			buff.arp.sender_ip_addr[idx];
		buff.arp.sender_ip_addr[idx] =
			ixrip_ip_addr[idx];
	}
	ethernet_send( 42 );
	DEBUG_FUNC_EXIT();
}

void arp_handle( void ){
	DEBUG_FUNC_ENTER();
	PDEBUG( "ip is %d.%d.%d.%d\n", 
			buff.arp.receiver_ip_addr[0],
			buff.arp.receiver_ip_addr[1],
			buff.arp.receiver_ip_addr[2],
			buff.arp.receiver_ip_addr[3] );

	uint16_t opcode = buff.arp.opcode;
	if( ! ipIsEqual( buff.arp.receiver_ip_addr, ixrip_ip_addr ) ){
		PDEBUG( "is not my ip\n" );
		DEBUG_FUNC_EXIT();
		return;
	}

	if( opcode == ARP_OPCODE_ANSWER ){
		// update arp cache
		arp_updateCache(
				buff.arp.sender_hw_addr, 
				buff.arp.sender_ip_addr
			       );
	}
	else if( opcode == ARP_OPCODE_REQUEST ){
		arp_updateCache( buff.arp.sender_hw_addr,
				buff.arp.sender_ip_addr
			       );
		arp_answer();
	}

	DEBUG_FUNC_EXIT();
}

/* 在ARP cache 中有一个优先级字段，最低的优先级
 * 将在有新的条目加入时被替换 */
uint8_t arp_cache_prio_max;

void arp_updateCache( uint8_t *mac, uint8_t *ip ){
	DEBUG_FUNC_ENTER();

	uint8_t idx;
	uint8_t min_prio;
	uint8_t min_item;

#if 0
	uint8_t temp;
	for( idx=0; idx<ARP_CACHE_SIZE; ++idx ){
		if( ip_is_equal(ip,arp_caches[idx].ip ) )
			break;
	}
	if( idx == ARP_CACHE_SIZE ){
		min_prio = arp_caches[0].prio;
		min_item = 0;
		for( idx=1; idx<ARP_CACHE_SIZE; ++idx ){
			temp = arp_caches[idx].prio;
			if( temp < min_prio ){
				min_item = idx;
				min_prio = temp;
			}
		}
		ipCopy( arp_caches[idx].ip, ip );
	}
#endif
	min_item = 0;
	min_prio = 0xff;
	for( idx=0; idx<ARP_CACHE_SIZE; ++idx ){
		uint8_t temp = arp_caches[idx].prio;
		if( temp < min_prio ){
				min_item = idx;
				min_prio = temp;
		}
		if( ipIsEqual( ip, arp_caches[idx].ip ) )
				break;
	}
	if( idx == ARP_CACHE_SIZE ){
		// 不存在
		idx = min_item;
		ipCopy( arp_caches[idx].ip, ip );
	}
	/* ****************** */
	macCopy( arp_caches[idx].mac, mac );
	arp_caches[idx].prio =  ++arp_cache_prio_max;
	/* 如果arp_cache_prio_max 超过最大值，把所有条目的优先级
	 * 设为0
	 */
	if( arp_cache_prio_max == 0xff ){
		for( idx=0; idx<ARP_CACHE_SIZE; ++idx ){
			arp_caches[idx].prio = 0;
		}
		arp_cache_prio_max = 0;
	}


	#ifdef ARP_DEBUG
	  arp_paintCaches();
	#endif

	DEBUG_FUNC_EXIT();
}

uint8_t *arp_findMacInCache( uint8_t *ip ){
	DEBUG_FUNC_ENTER();
	
	uint8_t idx;
	for( idx=0; idx<ARP_CACHE_SIZE; ++idx ){
		if( ipIsEqual( ip, arp_caches[idx].ip ) ){
			DEBUG_FUNC_EXIT();
			return arp_caches[idx].mac;
		}
	}
	PDEBUG( "not found\n" );
	DEBUG_FUNC_EXIT();
	return 0;
}

/* Print arp cache table */
void paint_macAddr( uint8_t *addr ){
	PDEBUG( "%x.%x.%x.%x.%x.%x",
			addr[0], addr[1],
			addr[2], addr[3],
			addr[4], addr[5] );
}
void paint_ipAddr( uint8_t *addr ){
	PDEBUG( "%x.%x.%x.%x",
			addr[0], addr[1],
			addr[2], addr[3] );
}
void arp_paintCaches( void ){
	uint8_t idx;
	for( idx=0; idx<ARP_CACHE_SIZE; ++idx ){
		paint_macAddr( arp_caches[idx].mac );
		PDEBUG_OUTPUT( "  " );
		paint_ipAddr( arp_caches[idx].ip );
		PDEBUG_OUTPUT( "  " );
		PDEBUG( "%d\n", arp_caches[idx].prio );
	}
}
