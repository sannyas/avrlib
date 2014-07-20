#ifndef _H_PDIUSBD12_USB_
#define _H_PDIUSBD12_USB_

#include <stdint.h>

/*******************************************
 * USB Request
 ******************************************/
typedef struct USBRequest{
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} usb_request_t;


/* bmRequestType[8]Data transfer direction */
#define getTransferDir(bm)	(bm&0x80)
#define DIR_HOST_TO_DEV		0x00
#define DIR_DEV_TO_HOST		0x80

/* bmRequestType[6:5]: Request type*/
#define getRequestType(bm)	(bm&0x60)
#define REQ_TYPE_STANDARD	0x00
#define REQ_TYPE_CLASS		0x20
#define REQ_TYPE_VENDER		0x40

/* bmRequestType[4:0]: Receiver of request */
#define getReqReceiver(bm)	(bm&0x1f)
#define RECEIVER_DEVICE		0x00
#define RECEIVER_INTERFACE	0x01
#define RECEIVER_ENDPOINT	0x02
#define RECEIVER_OTHER		0x03

/* bRequest: request code */
#define REQ_CODE_GET_STATUS		0
#define REQ_CODE_CLEAR_FEATURE		1
#define REQ_CODE_SET_FEATURE		3
#define REQ_CODE_SET_ADDRESS		5
#define REQ_CODE_GET_DESCRIPTOR		6
#define REQ_CODE_SET_DESCRIPTOR		7
#define REQ_CODE_GET_CONFIGURATION	8
#define REQ_CODE_SET_CONFIGURATION	9
#define REQ_CODE_GET_INTERFACE		10
#define REQ_CODE_SET_INTERFACE		11
#define REQ_CODE_SYNCH_FRAME		12


/* class request code */
#define REQ_CODE_SET_IDLE		0x0a


/******************************************
 * Device Descriptor
 *****************************************/
typedef struct USBDeviceDescriptor{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} usb_dev_descriptor_t;

/* The type of descriptor */
#define DESCRIPTOR_TYPE_DEVICE		1
#define DESCRIPTOR_TYPE_CONFIG		2
#define DESCRIPTOR_TYPE_STRING		3
#define DESCRIPTOR_TYPE_INTERFACE	4
#define DESCRIPTOR_TYPE_ENDPOINT	5

#define DESCRIPTOR_TYPE_REPORT		0x22

#define USB_VERSION_20			0x0200
#define USB_VERSION_11			0x0110

/*************************************
 * Configuration descriptor
 ************************************/
typedef struct USBConfigDescriptor{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} usb_conf_descriptor_t;

/************************************
 * Interface descriptor
 ***********************************/
typedef struct USBInterfaceDescriptor{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t numEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} usb_interface_descriptor_t;

/* subclass of interface */
#define INTERFACE_SUBCLASS_NONE	0
#define INTERFACE_SUBCLASS_BOOT	1

/* protocol code of interface */
#define INTERFACE_PROTOCOL_NONE		0
#define INTERFACE_PROTOCOL_KEYBOARD	1
#define INTERFACE_PROTOCOL_MOUSE	2

/***********************************
 * Endpoint descriptor
 ***********************************/
typedef struct USBEndpointDescriptor{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPackSize;
	uint8_t bInterval;
} usb_endp_descriptor_t;

/**************************************
 * Buffer
 **************************************/
typedef union USBBuffer{
	uint8_t data[18];
	usb_request_t request;
	usb_dev_descriptor_t dev_desc;
} usb_buffer_t;


/* Global */
extern usb_buffer_t usb_buff;
extern uint8_t send_len;
extern uint8_t *send_data;

/* isr */
void d12_isrHandle( void );
void d12_isrCtrlOut( void );
void d12_isrCtrlIn( void );

/* Functions */
void d12_sendEndpoint( );
void d12_requestHandle( void );
void d12_stdReqHandle( void );
void d12_classReqHandle( void );
void d12_getDescriptorHandle( void );
void d12_fillConfigDescriptorSet( uint8_t *data ); /* TODO */
uint8_t d12_enumCompleted( void );

/*
 * 此函数完成各种描述符的填充, 为了提供更大的灵活性, 该函数在具体
 * 的应用里定义, type参数为描述符的类型, idx作为索引或其他任意用
 * 途的数据, data参数为为该模块分配的Buffer空间, 该空间的大小
 * 可以通过宏在编译时配置, pos参数指定在该描述符中的起始位置, 
 * max_len指定data的最大以写入的数量.该函数返回写入data中的数据的
 * 数量.
 */
#define SEND_TYPE_DATA		0x01	// 数据在data指针中,直接发送 
#define SEND_TYPE_DEVICE	0x02	// 发送设备描述符, 调用fillDescriptor 
#define SEND_TYPE_CONFIG_SET	0x03	// 发送配置描述符集合,调用fillDescriptor 
#define SEND_TYPE_LANG_ID	0x04	// 发送语言ID, 调用fillDescriptor
#define SEND_TYPE_STRING	0x05
#define SEND_TYPE_REPORT	0x06
extern uint8_t d12_fillDescriptor( uint8_t type, uint8_t idx, uint8_t *data, uint8_t pos, uint8_t max_len );

#endif
