#ifndef _H_UDP_
#define _H_UDP_

#define UDP_HEADER_LEN		8
#define UDP_HEADER_OFFSET	34


typedef struct UDPDatagram{
	uint8_t padding[ UDP_HEADER_OFFSET ];
	uint16_t src_port;
	uint16_t dest_port;
	uint16_t len;
	uint16_t checksum;
	uint8_t data[1];
} udp_datagram_t;

/*
extern inline void udp_setDestIP( uint8_t *addr ){
	ipCopy( buff.ip.dest_addr, addr );
}

extern inline void udp_setPort( uint16_t src_port, uint16_t dest_port ){
	buff.udp.dest_port = dest_port;
	buff.udp.src_port = src_port;
}

extern inline void udp_setDataLength( uint16_t len ){
	buff.udp.len = len + 8;
}

extern inline uint8_t *udp_getDataPtr( void ){
	return buff.udp.data;
}
*/

uint8_t udp_send( uint16_t len );

void udp_handle( void );

#endif
