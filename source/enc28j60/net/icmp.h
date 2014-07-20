
#define ICMP_PACKET_PADDING 34
#define ICMP_PACKET_OFFSET	34

typedef struct ICMPPacket{
	uint8_t icmp_padding[ICMP_PACKET_PADDING];
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint8_t data[1];
}icmp_packet_t;

#define ICMP_TYPE_ECHO_REPLY		0
#define ICMP_TYPE_ECHO_REQUEST		8
#define ICMP_TYPE_TIMESTAMP		13

void icmp_handle( void );
