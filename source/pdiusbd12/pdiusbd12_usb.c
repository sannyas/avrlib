#include <stdio.h>
#include "pdiusbd12_usb.h"
#include "pdiusbd12.h"

#ifdef PDIUSBD12_DEBUG
#define AVRLIB_DEBUG
#endif
#include <debug.h>

/* Global variable */
usb_buffer_t usb_buff;
static uint8_t enum_completed = 0;

/* ******************************** */
// Send to Endpoint
/* ******************************** */

uint8_t u_send_type;
uint8_t *u_send_data;	// 为NULL则不发送
uint8_t u_send_len;	// 还需要发送的数据的长度, 如果data不为NULL, len为0, 则发送一个长度为0的包
uint8_t u_send_endp;	// 数据发送到哪一个Endpoint 
uint8_t u_send_pos;	// 在fillDescriptor中的起始位置 
uint8_t u_send_idx;	// 在fillDescriptor中指定索引或其他数据 

/*
#define SEND_TYPE_DATA		0x01	// 数据在data指针中,直接发送 
#define SEND_TYPE_DEVICE	0x02	// 发送设备描述符, 调用fillDescriptor 
#define SEND_TYPE_CONFIG_SET	0x03	// 发送配置描述符集合,调用fillDescriptor 
#define SEND_TYPE_LANG_ID	0x04
#define SEND_TYPE_STRING	0x05
*/

/*
 * 直接发送数据, 写入:
 * 	u_send_type = SEND_TYPE_DATA
 * 	u_send_data = 数据指针
 * 	u_send_len = 数据长度
 * 	u_send_endp = 发送的Endp
 *
 * 通过fillDescriptor发送数据,写入:
 * 	u_send_type = 指定描述符
 * 	u_send_data = Buffer首地址
 * 	u_send_endp = 发送的Endp
 * 	u_send_pos = 初始化为0
 * 	u_send_idx = 特定数据,可以不使用
 * 	u_send_len = 数据的最大长度
 */

void d12_sendEndpoint( ){
	uint8_t ret;
	uint8_t max_len;
	uint8_t len;

	/* 如果data为NULL,直接返回*/
	if( u_send_data == NULL )
		return;

	d12_selectEndpoint( u_send_endp );	// 选择发送的Endpoint

	if( u_send_endp == D12_ENDP_CTRL_IN )
		max_len = 16;
	if( max_len < D12_BUFFER_SIZE )
		max_len = D12_BUFFER_SIZE;

	/* TODO else if( */

	if( u_send_type != SEND_TYPE_DATA ){ // 需要从fillDescriptor中获取数据
		ret = d12_fillDescriptor(
				u_send_type,
				u_send_idx,
				u_send_data,
				u_send_pos,
				max_len
			);
		u_send_pos += ret;
		if( ret < max_len && ret < u_send_len)
			u_send_len = ret;
	}

	/**********************
	 * 发送数据
	 *********************/
	if( u_send_len == 0 ){ // 发送一个长度为0的包
		d12_writeBuffer( D12_ENDP_CTRL_IN, 0, 0 ); // Empty
		d12_validateBuffer();
		u_send_data = NULL;
		return;
	}

	len = u_send_len;
	if( len > max_len )
		len = max_len;
	d12_writeBuffer( D12_ENDP_CTRL_IN, u_send_data, len );
	d12_validateBuffer();

	u_send_len -= len;
	if( u_send_type == SEND_TYPE_DATA )
		u_send_data += len;
	if( len != max_len ) // 发送完成
		u_send_data = NULL;
}

void d12_stdReqHandle( void ){
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint8_t wLength;
	uint16_t wIndex;

	bmRequestType = usb_buff.request.bmRequestType;
	bRequest = usb_buff.request.bRequest;
	wValue = usb_buff.request.wValue;
	wLength = usb_buff.request.wLength;
	wIndex = usb_buff.request.wIndex;

	/* Device to host */
	if( getTransferDir(bmRequestType) == DIR_DEV_TO_HOST ){
		switch( bRequest ){
		case REQ_CODE_GET_STATUS:
			break;
		case REQ_CODE_GET_DESCRIPTOR:
			PDEBUG_OUTPUT( "get descriptor\n" );
			switch( (wValue>>8)&0xff ){
			case DESCRIPTOR_TYPE_DEVICE:
				u_send_type = SEND_TYPE_DEVICE;
				u_send_data = usb_buff.data;
				u_send_endp = D12_ENDP_CTRL_IN;
				u_send_pos = 0;
				u_send_len = wLength;
				d12_sendEndpoint();
				break;
			case DESCRIPTOR_TYPE_CONFIG: // Get configuration descriptor
				u_send_type = SEND_TYPE_CONFIG_SET;
				u_send_data = usb_buff.data;
				u_send_endp = D12_ENDP_CTRL_IN;
				u_send_pos = 0;
				u_send_len = wLength;
				d12_sendEndpoint();
				break;
			case DESCRIPTOR_TYPE_STRING: // Get string
				if( wIndex == 0 ){ // Request Language ID
					u_send_type = SEND_TYPE_LANG_ID;	
					u_send_data = usb_buff.data;
					u_send_endp = D12_ENDP_CTRL_IN;
					u_send_pos = 0;
					u_send_len = wLength;
					d12_sendEndpoint();
				}
				else{
					u_send_type = SEND_TYPE_STRING;
					u_send_data = usb_buff.data;
					u_send_endp = D12_ENDP_CTRL_IN;
					u_send_pos = 0;
					u_send_len = wLength;
					u_send_idx = wValue&0xff;
					d12_sendEndpoint();
				}
				break;
			case DESCRIPTOR_TYPE_REPORT:
				u_send_type = SEND_TYPE_REPORT;	
				u_send_data = usb_buff.data;
				u_send_endp = D12_ENDP_CTRL_IN;
				u_send_pos = 0;
				u_send_len = wLength;
				d12_sendEndpoint();
				break;

			}
			break;
		case REQ_CODE_GET_CONFIGURATION:
			break;
		case REQ_CODE_GET_INTERFACE:
			break;
		case REQ_CODE_SYNCH_FRAME:
			break;
		}
	}
	/* Host to device */
	else{
		switch( bRequest ){
		case REQ_CODE_CLEAR_FEATURE:
			break;
		case REQ_CODE_SET_FEATURE:
			break;
		case REQ_CODE_SET_ADDRESS:
			d12_setAddress( wValue&0xff, 1 );
			u_send_type = SEND_TYPE_DATA;
			u_send_data = usb_buff.data;
			u_send_len = 0;
			d12_sendEndpoint();
			break;
		case REQ_CODE_SET_DESCRIPTOR:
			break;
		case REQ_CODE_SET_CONFIGURATION:	// Set Configuration
			d12_enableEndp(wValue&0xff);
			u_send_type = SEND_TYPE_DATA;
			u_send_data = usb_buff.data;
			u_send_len = 0;
			d12_sendEndpoint();
			break;
		case REQ_CODE_SET_INTERFACE:
			break;
		}
	}
}

void d12_classReqHandle( void ){
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint8_t wLength;
	uint16_t wIndex;

	bmRequestType = usb_buff.request.bmRequestType;
	bRequest = usb_buff.request.bRequest;
	wValue = usb_buff.request.wValue;
	wLength = usb_buff.request.wLength;
	wIndex = usb_buff.request.wIndex;

	switch( bRequest ){
	case REQ_CODE_SET_IDLE:
		/* return a empty packet */
		u_send_type = SEND_TYPE_DATA;
		u_send_data = usb_buff.data;
		u_send_len = 0;
		d12_sendEndpoint();
		break;
	}
}

void d12_requestHandle( void ){
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint8_t wLength;
	uint16_t wIndex;

	bmRequestType = usb_buff.request.bmRequestType;
	bRequest = usb_buff.request.bRequest;
	wValue = usb_buff.request.wValue;
	wLength = usb_buff.request.wLength;
	wIndex = usb_buff.request.wIndex;

	/* Device to host */
	switch( getRequestType(bmRequestType) ){
	case REQ_TYPE_STANDARD:
		d12_stdReqHandle();
		break;
	case REQ_TYPE_CLASS:
		d12_classReqHandle();
		break;
	}

}

void d12_isrHandle( void ){
	uint8_t intr;
	uint8_t status;
	
	if( d12_pin_INT_getValue() == 1 )
		return;
	intr = d12_getInterrupt( );

	if( intr & INTR_SUSPEND_CHANGE ){
		PDEBUG_OUTPUT( "int: suspend ch\n" );
	}
	else if( intr & INTR_BUS_RESET ){
		PDEBUG_OUTPUT( "int: bus reset\n" );
	}
	else if( intr & INTR_CTRL_OUT ){
		PDEBUG_OUTPUT( "int: ctrl out\n" );
		d12_isrCtrlOut( );
	}
	else if( intr & INTR_CTRL_IN ){
		PDEBUG_OUTPUT( "int: ctrl in\n" );
		d12_isrCtrlIn();
	}
	else if( intr & INTR_ENP1_OUT ){
		PDEBUG_OUTPUT( "int: 1 out\n" );
	}
	else if( intr & INTR_ENP1_IN ){
		PDEBUG_OUTPUT( "int: 1 in\n" );

	}
	else if( intr & INTR_MAIN_OUT ){
		PDEBUG_OUTPUT( "int: 2 out\n" );

	}
	else if( intr & INTR_MAIN_IN ){
		PDEBUG_OUTPUT( "int: 2 in\n" );
	}
}

void d12_isrCtrlOut( void ){
	uint8_t status;
	uint8_t buf_size;

	status = d12_getLastTransStatus( D12_ENDP_CTRL_OUT );
	PDEBUG( "status=%x\n", status );
	
	buf_size = d12_readBuffer( D12_ENDP_CTRL_OUT, usb_buff.data, 16 );
	if( status & 0x20 ){ // setup packet
		d12_acknowledgeSetup();
		d12_clearBuffer();
		d12_requestHandle();
	}
	else{
		/*
		if( buf_size == 0 ){
			enum_completed = 1;
			PDEBUG( "enum is completed\n" );
		}
		*/
	}
}

void d12_isrCtrlIn( void ){
	uint8_t status;
	status = d12_getLastTransStatus( D12_ENDP_CTRL_IN );
	d12_sendEndpoint();
}

uint8_t d12_enumCompleted( void ){
	return enum_completed;
}
