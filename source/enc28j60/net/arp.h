/*
 * 在上层调用 arp_request
 * 在下层调用 arp_handle
 */
#ifndef __H_ARP__
#define __H_ARP__

#include "conf.h"

/*  ****************************** */
// Configurations
/*  ****************************** */
#define IXRIP_BYTE_ORDER	LIGHT_ENDIAN // BIG_ENDIAN

/* ****************************** */
// Type define
/* ****************************** */

/* ARP packet format */
typedef struct ArpPacket{
	uint8_t __padding[14]; // Ethernet header
	uint16_t hardware_type;
	uint16_t protocal_type;
	uint8_t hardware_addr_len;
	uint8_t protocal_addr_len;
	uint16_t opcode;
	uint8_t sender_hw_addr[6];
	uint8_t sender_ip_addr[4];
	uint8_t receiver_hw_addr[6];
	uint8_t receiver_ip_addr[4];
} arp_packet_t;

/* ARP Cache Struct */
typedef struct ArpCache{
	uint8_t mac[6];
	uint8_t ip[4];
	uint8_t prio;
} arp_cache_t;


/* ****************************** */
// Const 
/* ****************************** */
#if IXRIP_BYTE_ORDER == LIGHT_ENDIAN

  #define ARP_HARDWARE_TYPE_ETHERNET	0x0100
  #define ARP_PROTOCAL_TYPE_IP		0x0008
  #define ARP_OPCODE_REQUEST		0x0100
  #define ARP_OPCODE_ANSWER		0x0200

#elif IXRIP_BYTE_ORDER == BIG_ENDIAN

  #define ARP_HARDWARE_TYPE_ETHERNET	0x0001
  #define ARP_PROTOCAL_TYPE_IP		0x0800
  #define ARP_OPCODE_REQUEST		0x0001
  #define ARP_OPCODE_ANSWER		0x0002

#else
  #error Byte order is not defined
#endif


arp_cache_t arp_caches[ ARP_CACHE_SIZE ];


/* ****************************** */
// Operations
/* ****************************** */

/*
 * 发送一个ARP请求，此函数先在ARP cache中查询是否有
 * 需要的地址， 如果没有， 发送一个参数中指定地址的
 * 请求， 然后返回一个空指针
 */
uint8_t *arp_request( uint8_t *ip_addr );

/*
 * 相应一个ARP请求
 */
void arp_answer ( void );

/*
 * 当以太网层收到一个ARP协议的帧， 调用该函数进行处理
 */
void arp_handle( void );

/*
 * 更新cache
 */
void arp_updateCache( uint8_t *mac, uint8_t *ip );

/* 
 * 在cache中查找 
 */ 
uint8_t *arp_findMacInCache( uint8_t *ip );
//uint8_t *arp_findIpInCache( uint8_t *mac );

#endif
