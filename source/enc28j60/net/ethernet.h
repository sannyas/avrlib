#ifndef __H_ETHERNET__
#define __H_ETHERNET__

#define ETHERNET_HEADER_LEN	14

typedef struct EthernetFrame{
	uint8_t dest_addr[6];
	uint8_t src_addr[6];
	uint16_t len_or_type;
	uint8_t data[1];
} ethernet_frame_t;

#if IXRIP_BYTE_ORDER == LIGHT_ENDIAN
  #define ETHERNET_TYPE_ARP 0x0608
  #define ETHERNET_TYPE_IP 0x0008
#elif IXRIP_BYTE_ORDER == BIG_ENDIAN
  #define ETHERNET_TYPE_ARP 0x0806
  #define ETHERNET_TYPE_IP 0x0800
#else
  #error ETHERNET_TYPE_ARP
#endif

void ethernet_send( uint16_t len );
void ethernet_handle( void );

#endif
