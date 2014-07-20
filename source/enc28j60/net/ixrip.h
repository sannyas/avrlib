#ifndef __H_IXRIP__
#define __H_IXRIP__

/* include header file */
#include <stdint.h>
#include "arp.h"
#include "ethernet.h"
#include "ip.h"
#include "conf.h"
#include "icmp.h"
#include "udp.h"

/* Set ip and mac address */
extern uint8_t *ixrip_ip_addr;
extern uint8_t *ixrip_mac_addr;

extern inline void ixrip_setIp( uint8_t *ip ){
	ixrip_ip_addr = ip;
}
extern inline void ixrip_setMac( uint8_t *mac ){
	ixrip_mac_addr = mac;
}

typedef union IxrNet{
	uint8_t bytes[ IXRIP_BUFFER_LEN ];
	ethernet_frame_t	ethernet;
	arp_packet_t		arp;
	ip_packet_t		ip;
	icmp_packet_t		icmp;
	udp_datagram_t		udp;
} ixr_net_t;
extern ixr_net_t buff;

/* ÔÓÏîº¯Êý */
uint8_t ipIsEqual( uint8_t const *ip1, uint8_t const *ip2 );
uint8_t macIsEqual( uint8_t const *mac1, uint8_t const *mac2 );
void ipCopy( uint8_t *dest_ip, uint8_t const *src_ip );
void macCopy( uint8_t *dest_mac, uint8_t const *src_mac );
uint16_t checksum( uint16_t sum, const uint8_t *data, uint16_t len );

uint16_t swap_16( uint16_t data );


#endif
