#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

#ifdef MFRC522_DEBUG
  #define AVRLIB_DEBUG
#endif
#include <debug.h>

#include "mfrc522.h"

#define mfrc522_delay_ms(n) _delay_ms(n)
#define mfrc522_delay_us(n) _delay_us(n)

/* spi read and write */
static uint8_t __spi_rw( uint8_t data ){
	uint8_t idx;
	uint8_t rd;

	rd = 0;
	for( idx=0; idx<8; ++idx ){
		mfrc522_pin_MOSI_set( data&0x80 );
		data <<= 1;
		mfrc522_delay_us( 1 );
		mfrc522_pin_SCLK_set( 1 );
		mfrc522_delay_us( 1 );
		rd <<= 1;
		if( mfrc522_pin_MISO_get() )
			rd |= 0x01;
		mfrc522_pin_SCLK_set( 0 );
		mfrc522_delay_us( 1 );
	}

	return rd;
}

/* spi read a byte data from mfrc522 */
static uint8_t __spi_readByte( uint8_t addr ){
	uint8_t data;

	mfrc522_pin_NSS_set( 0 );
	mfrc522_delay_us( 1 );

	__spi_rw( addr | 0x80 );
	data = __spi_rw( 0 );

	mfrc522_pin_NSS_set( 1 );
	mfrc522_delay_ms( 1 );
	return data;
}

/* spi write a byte data to mfrc522 */
static void __spi_writeByte( uint8_t addr, uint8_t data ){
	mfrc522_pin_NSS_set( 0 );
	mfrc522_delay_us( 1 );

	__spi_rw( addr );
	__spi_rw( data );

	mfrc522_pin_NSS_set( 1 );
}

/* write a array of data to mfrc522 */
static uint8_t __spi_writeBytes( uint8_t addr, uint8_t *data, uint8_t count ){
	uint8_t idx;

	mfrc522_pin_NSS_set( 0 );
	mfrc522_delay_us( 1 );

	__spi_rw( addr );
	for( idx=0; idx<count; ++idx ){
		__spi_rw( data[idx] );
	}

	mfrc522_pin_NSS_set( 1 );
	return idx;	
}

/* read data from register */
static uint8_t __mfrc522_readReg( uint8_t reg ){
	uint8_t data;
	data = __spi_readByte( reg<<1 );
	return data;
}

/* write data to register */
static void __mfrc522_writeReg( uint8_t reg, uint8_t data ){
	__spi_writeByte( reg<<1, data );
}

/* Set bits of regiister with specific mask */
static void __mfrc522_setBitsOfReg( uint8_t reg, uint8_t mask ){
	uint8_t val_reg;
	val_reg = __mfrc522_readReg( reg );
	val_reg |= mask;
	__mfrc522_writeReg( reg, val_reg );
}

/* clear bits of register with specific mask */
static void __mfrc522_clearBitsOfReg( uint8_t reg, uint8_t mask ){
	uint8_t val_reg;
	val_reg = __mfrc522_readReg( reg );
	val_reg &= ~mask;
	__mfrc522_writeReg( reg, val_reg );
}

void mfrc522_reset( void ){
	mfrc522_pin_RST_set( 1 );
	mfrc522_delay_us( 5 );
	mfrc522_pin_RST_set( 0 );
	mfrc522_delay_us( 5 );
	mfrc522_pin_RST_set( 1 );

	__mfrc522_writeReg( MFRC522_REG_Command, MFRC522_CMD_SoftReset );
	mfrc522_delay_ms( 10 );
}
void mfrc522_init( void ){
	mfrc522_pin_init();
	mfrc522_reset();

	__mfrc522_writeReg( MFRC522_REG_TMode, 0x82 );
	__mfrc522_writeReg( MFRC522_REG_TPrescaler, 0xa6 );

	mfrc522_delay_ms( 1 );
}

/* TODO */
uint8_t mfrc522_setISOType( uint8_t type ){
	DEBUG_FUNC_ENTER();

	if( type == ISO14443_TYPE_A ){ //ISO14443_A 
		PDEBUG( "  ISO1443_A\n" );
		__mfrc522_clearBitsOfReg( MFRC522_REG_Status2, MFRC522_MFCrypto1On );
		/*
		WriteRawRC(CommandReg,0x20);    //as default   
		WriteRawRC(ComIEnReg,0x80);     //as default
		WriteRawRC(DivlEnReg,0x0);      //as default
		WriteRawRC(ComIrqReg,0x04);     //as default
		WriteRawRC(DivIrqReg,0x0);      //as default
		WriteRawRC(Status2Reg,0x0);//80    //trun off temperature sensor
		WriteRawRC(WaterLevelReg,0x08); //as default
		WriteRawRC(ControlReg,0x20);    //as default
		WriteRawRC(CollReg,0x80);    //as default
		*/

		/* 设置CRC协处理器的预置值， Mifare卡通信定义为0x6363 */
		__mfrc522_writeReg( MFRC522_REG_Mode, 0x3d ); // default is 0x3f

		/*
		WriteRawRC(TxModeReg,0x0);      //as default???
		WriteRawRC(RxModeReg,0x0);      //as default???
		WriteRawRC(TxControlReg,0x80);  //as default???
		WriteRawRC(TxSelReg,0x10);      //as default???
   		*/
		__mfrc522_writeReg( MFRC522_REG_RxSel, 0x86 ); // default is 0x84
		/*
 		WriteRawRC(RxThresholdReg,0x84);//as default
 		WriteRawRC(DemodReg,0x4D);      //as default
		WriteRawRC(ModWidthReg,0x13);//26
		*/
		__mfrc522_writeReg( MFRC522_REG_RFCfg, 0x7f ); // default is 0x4f, 接收器的电压增益48dB
		/*
		WriteRawRC(GsNReg,0x88);        //as default???
		WriteRawRC(CWGsCfgReg,0x20);    //as default??
		WriteRawRC(ModGsCfgReg,0x20);   //as default???
		*/

		/*
		__mfrc522_writeReg( MFRC522_REG_TReload_L, 30 );
		__mfrc522_writeReg( MFRC522_REG_TReload_H, 0 );
		__mfrc522_writeReg( MFRC522_REG_TMode, 0x8d );
		__mfrc522_writeReg( MFRC522_REG_TPrescaler, 0x3e );
		*/
		__mfrc522_writeReg( MFRC522_REG_TxAuto, 0x40 );
		__mfrc522_writeReg( MFRC522_REG_Mode, 0x30 );

	   
		//PcdSetTmo(106);
		_delay_ms( 10 );
		mfrc522_setAntennaPower( 1 );
	}
	else{
		return -1;
	}
   	return 1;

	DEBUG_FUNC_EXIT();
}

/* Set the reload value of timer */
void mfrc522_timer_setCount( uint16_t cnt ){
	__mfrc522_writeReg( MFRC522_REG_TReload_H, (uint8_t)(cnt>>8) );
	__mfrc522_writeReg( MFRC522_REG_TReload_L, (uint8_t)(cnt&0xff) );
}

/* Start timer */
void mfrc522_timer_start( ){
	__mfrc522_setBitsOfReg( MFRC522_REG_Control, MFRC522_TStartNow );
}

/* Stop timer */
void mfrc522_timer_stop( void ){
	__mfrc522_setBitsOfReg( MFRC522_REG_Control, MFRC522_TStopNow );
}

/* Set preset value of CRC */
void mfrc522_crc_setPreset( uint8_t preset ){
	uint8_t reg_val;
	reg_val = __mfrc522_readReg( MFRC522_REG_Mode );
	reg_val &= ~0x03;
	reg_val |= preset;
	__mfrc522_writeReg( MFRC522_REG_Mode, reg_val );
}

/* Execute ClacCRC command */
void mfrc522_crc_startCalc( void ){
	__mfrc522_writeReg( MFRC522_REG_Command, MFRC522_CMD_CalcCRC );
}

/* Write data to FIFOData */
void mfrc522_crc_calcData( uint8_t data ){
	__mfrc522_writeReg( MFRC522_REG_FIFOData, data );
}

/* Write data to FIFOData */
void mfrc522_crc_calcArray( uint8_t *data, uint8_t len ){
	uint8_t idx;
	for( idx=0; idx<len; ++idx )
		__mfrc522_writeReg( MFRC522_REG_FIFOData, data[idx] );
}

/* stop crc */
void mfrc522_crc_stop( void ){
	__mfrc522_writeReg( MFRC522_REG_Command, MFRC522_CMD_Idle );
}

/* Get result of CRC */
uint16_t mfrc522_crc_getResult( void ){
	uint16_t temp;
	temp = __mfrc522_readReg( MFRC522_REG_CRCResult_H );
	temp <<= 8;
	temp += __mfrc522_readReg( MFRC522_REG_CRCResult_L );
	return temp;
}

/* Execute transcceie command */
uint8_t mfrc522_transceive( mfrc522_transceive_data_t *data ){
	DEBUG_FUNC_ENTER();

	uint8_t wait_for = 0x30;// wait fo irq: Rx,Idle
	uint8_t temp;
	uint8_t idx;
	uint8_t ret;

	__mfrc522_writeReg( MFRC522_REG_ComIEn, 0x77 );// Tx,Rx,Idle,LoAlert,Err,Timer
	//__mfrc522_clearBitsOfReg( MFRC522_REG_ComIrq, 0x80 );
	__mfrc522_writeReg( MFRC522_REG_Command, MFRC522_CMD_Idle );
	__mfrc522_setBitsOfReg( MFRC522_REG_FIFOLevel, 0x80 );//reset fifo

	
	__mfrc522_writeReg( MFRC522_REG_ComIrq, 0x7f );
	for (idx=0; idx<data->data_send_len; idx++){
		__mfrc522_writeReg( MFRC522_REG_FIFOData,
				data->data_send[idx] );
	}
	temp = __mfrc522_readReg( MFRC522_REG_FIFOLevel );
	PDEBUG( "filo level: %d\n", temp );

	mfrc522_timer_setCount( 250 ); // The max time to wait, 25 ms 
	__mfrc522_writeReg( MFRC522_REG_Command, MFRC522_CMD_Transceive );
	__mfrc522_setBitsOfReg( MFRC522_REG_BitFraming, MFRC522_StartSend );

	do {
		temp = __mfrc522_readReg( MFRC522_REG_ComIrq );
		PDEBUG( "irq = %x\n", temp );
	}
	while( !(temp&0x01) && !(temp&wait_for));
	__mfrc522_clearBitsOfReg( MFRC522_REG_BitFraming, 0x80 );

	idx = __mfrc522_readReg( MFRC522_REG_Error );
	if( idx & 0x1B ){
		PDEBUG( "ErrorReg: %x\n", idx );
		ret = MFRC522_RET_CODE_ERROR; //Error
	}
	else if( temp & 0x01 ){
		ret = MFRC522_RET_CODE_NOTAG;// Timeout
		PDEBUG( "recv data timeout\n" );
	}
	else{
		temp = __mfrc522_readReg( MFRC522_REG_FIFOLevel );
		idx = __mfrc522_readReg( MFRC522_REG_Control ) & 0x07; // lastBits
		PDEBUG( "recv data %d bytes\n", temp );
		data->data_recv_len = temp;
		data->last_bits = idx;
		for( idx=0; idx<temp; ++idx ){
			data->data_recv[idx] = __mfrc522_readReg( MFRC522_REG_FIFOData );
		}
		ret = MFRC522_RET_CODE_OK;
	}

	__mfrc522_writeReg( MFRC522_REG_ComIrq, 0x7f );
	__mfrc522_setBitsOfReg( MFRC522_REG_Control, 0x80 ); // stop timer
	__mfrc522_writeReg( MFRC522_REG_Command, MFRC522_CMD_Idle );

	DEBUG_FUNC_EXIT();
	return ret;
}

/* Power on or power off  the antenna*/
void mfrc522_setAntennaPower( uint8_t on ){
	uint8_t val_tx_conf;

	val_tx_conf = __mfrc522_readReg( MFRC522_REG_TxControl );
	if( on ){
		if( !(val_tx_conf & 0x03) )
			__mfrc522_writeReg( MFRC522_REG_TxControl,
					val_tx_conf | 0x03 );
	}
	else{
		__mfrc522_writeReg( MFRC522_REG_TxControl,
				val_tx_conf & ~(0x03) );
	}
}

/* TODO */
static uint8_t hex2dec( char ch ){
	if( ch < 'A' )
		return ch-'0';
	else
		return 10 + ( ch-'A' );
}

void mfrc522_test_rw( void ){
	char ch;
	uint8_t data1, data2, data3;
	printf( "test rw enter\n" );

	while( 1 ){
		ch = getchar();
		if( ch == 'r' ){
			ch = getchar();
			data1 = hex2dec(ch)<<4;
			ch = getchar();
			data1 += hex2dec( ch );
			data3 = __mfrc522_readReg( data1 );
			printf( "read reg %x: %x\n", data1, data3 );
		}
		else if( ch == 'w' ){
			ch = getchar();
			data1 = hex2dec(ch)<<4;
			ch = getchar();
			data1 += hex2dec( ch );

			ch = getchar();
			data2 = hex2dec(ch)<<4;
			ch = getchar();
			data2 += hex2dec( ch );

			__mfrc522_writeReg( data1, data2 );
			printf( "write %x to reg %x\n", data2, data1 );
		}
	}
}

void mfrc522_timer_test(){
	printf( "start test\n" );
	uint8_t cnt = 20;
	uint8_t temp;
	mfrc522_init();
	mfrc522_timer_setCount( 10000 );
	__mfrc522_writeReg( MFRC522_REG_ComIrq, 0x7f );
	printf( "irq %d\n", __mfrc522_readReg( MFRC522_REG_ComIrq ) );


	while( cnt > 0 ){
		mfrc522_timer_start();
		while( 1 ){
			temp = __mfrc522_readReg( MFRC522_REG_ComIrq );
			if( temp & MFRC522_TimerIRq )
				break;
		}
		__mfrc522_writeReg( MFRC522_REG_ComIrq, 0xff );
		printf( "Timer running: %d\n", cnt );
		--cnt;
		__mfrc522_writeReg( MFRC522_REG_ComIrq, 0x7f );
	}
	while(1){}

}

void mfrc522_test( ){
	uint8_t ret;
	uint16_t tag_type;
	mfrc522_init();
	mfrc522_setISOType( ISO14443_TYPE_A );

	while( 1 ){
		printf( "start find card...\n" );
		ret = card_request( 0x52, &tag_type );
		printf( "tag_type: %x\n", tag_type );
		if( ret == MFRC522_RET_CODE_OK ){
			printf( "find card ok\n" );
		}
		else{
			printf( "find card error\n" );
		}
		_delay_ms( 1000 );
	}
}

uint8_t card_request( uint8_t req_code, uint16_t *tag_type ){
	uint8_t ret;  
	//unsigned int  unLen;
	unsigned char buffer[8]; 
	__mfrc522_clearBitsOfReg( MFRC522_REG_Status2, MFRC522_MFCrypto1On );
	__mfrc522_writeReg( MFRC522_REG_BitFraming, 0x07 ); 


	buffer[0] = req_code;
	mfrc522_transceive_data_t data;
	data.data_send = buffer;
	data.data_send_len = 1;
	data.data_recv = buffer;
	data.data_recv_len = 0;
	data.last_bits = 7;
	ret = mfrc522_transceive( &data );

	//status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
	if( ret == MFRC522_RET_CODE_OK ){
		(*tag_type) = (buffer[0]<<8)+buffer[1];
	}	

	return ret;
}

