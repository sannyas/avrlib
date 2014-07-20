#include "ixrip.h"

ixr_net_t buff;
uint8_t *ixrip_ip_addr;
uint8_t *ixrip_mac_addr;

uint8_t ipIsEqual( uint8_t const *ip1, uint8_t const *ip2 ){
	return (
		ip1[0] == ip2[0] &&
		ip1[1] == ip2[1] &&
		ip1[2] == ip2[2] &&
		ip1[3] == ip2[3]
	       );
}
uint8_t macIsEqual( uint8_t const *mac1, uint8_t const *mac2 ){
	return (
			mac1[0] == mac2[0] &&
			mac1[1] == mac2[1] &&
			mac1[2] == mac2[2] &&
			mac1[3] == mac2[3] &&
			mac1[4] == mac2[4] &&
			mac1[5] == mac2[5]
	       );
}
void ipCopy( uint8_t *dest_ip, uint8_t const *src_ip ){
	dest_ip[0] = src_ip[0];
	dest_ip[1] = src_ip[1];
	dest_ip[2] = src_ip[2];
	dest_ip[3] = src_ip[3];
}

void macCopy( uint8_t *dest_mac, uint8_t const *src_mac ){
	dest_mac[0] = src_mac[0];
	dest_mac[1] = src_mac[1];
	dest_mac[2] = src_mac[2];
	dest_mac[3] = src_mac[3];
	dest_mac[4] = src_mac[4];
	dest_mac[5] = src_mac[5];
}

extern inline uint16_t checksum_add( uint16_t sum, uint16_t data ){
	sum += data;
	if( sum < data ){
		sum ++;
	}
	return sum;
}

uint16_t checksum( uint16_t sum, const uint8_t *data, uint16_t len ){
	uint16_t temp;

	while( len > 1 ){
		temp = (data[0]<<8)+data[1];
		sum += temp;
		if( sum < temp )
			sum ++;

		len -= 2;
		data += 2;
	}
	if( len > 0 ){
		temp = (data[0]<<8);
		sum += temp;
		if( sum < temp )
			sum ++;
	}
	return sum;
#if 0
	uint16_t t;
	const uint8_t *dataptr;
	const uint8_t *last_byte;

	dataptr = data;
	last_byte = data + len - 1;

	while( dataptr < last_byte ){	/* At least two more bytes */
		t = (dataptr[0] << 8) + dataptr[1];
		sum += t;
		if(sum < t) {
			sum++;		/* carry */
		}
		dataptr += 2;
	}

	if(dataptr == last_byte) {
		t = (dataptr[0] << 8) + 0;
		sum += t;
		if(sum < t) {
			sum++;		/* carry */
		}
	}

	/* Return sum in host byte order. */
	return sum;
#endif
}

uint16_t swap_16( uint16_t data ){
	return ( (data>>8) + (data<<8) );
}
