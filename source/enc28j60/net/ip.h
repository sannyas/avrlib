#ifndef _H_IP_
#define _H_IP_

#define IP_PACKET_PADDING	14
#define IP_HEADER_LEN		20
#define IP_PACKET_OFFSET	14

typedef struct IPPacket{
	uint8_t padding[14];
	uint8_t ver_and_hlen;
	uint8_t tos;
	uint16_t total_len;
	uint16_t identification;
	uint16_t flags_and_fragoff;
	uint8_t time_to_live;
	uint8_t protocal;
	uint16_t header_checksum;
	uint8_t src_addr[4];
	uint8_t dest_addr[4];
	uint8_t data[1];
} ip_packet_t;

#define IP_VERSION_4	4<<4;
#define IP_PROTOCAL_ICMP	1
#define IP_PROTOCAL_TCP		6
#define IP_PROTOCAL_UDP		17

/*
 * @brief IP header checksum
 *
 * @return IP_CHECKSUM_OK or IP_CHECKSUM_ERROR
 */
#define IP_CHECKSUM_OK	0
#define IP_CHECKSUM_ERROR 1
uint8_t ipHeaderChecksum( void );

/*
 * @brief set the checksum field.
 */
void setIpHeaderChecksum( void );

/*
 * @brief Send a IP packet.
 *
 * Call this function to send a IP packet. This
 * function doesn't need parameters, so before calling
 * this function should fill some necessary fields:
 * 	total length, 
 * 	Identification, 
 * 	protocal, 
 * 	destination address
 * 	data.
 *
 * @return  IP_SEND_*
 */
#define IP_SEND_OK	0x00
#define IP_SEND_NO_MAC	0x01
uint8_t sendIPPacket( void );

void ip_handle( void );

#endif
