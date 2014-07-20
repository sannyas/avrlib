/**
 * @file mfrc522.h
 * @author Liao Maoyi
 */
#ifndef _MFRC522_H_
#define _MFRC522_H_

#include <stdint.h>
/*@{*/
/** register */

/* Command and status */
#define MFRC522_REG_Reserved1	0x00
#define MFRC522_REG_Command	0x01 /* starts and stops command execution */
  #define MFRC522_RcvOff	(1<<5)
  #define MFRC522_PowerDown	(1<<4)
#define MFRC522_REG_ComIEn	0x02 /* enable and disable interrupt request control bits */
  #define MFRC522_IRqInv	(1<<7)
  #define MFRC522_TxIEn		(1<<6)
  #define MFRC522_RxIEn		(1<<5)
  #define MFRC522_IdleEn	(1<<4)
  #define MFRC522_HiAlertIEn	(1<<3)
  #define MFRC522_LoAlertIEn	(1<<2)
  #define MFRC522_ErrIEn	(1<<1)
  #define MFRC522_TimerIEn	(1<<0)
#define MFRC522_REG_DivIEn	0x03 /* enable and disable interrupt request control bits */
  #define MFRC522_IRQPusPull	(1<<7)
  #define MFRC522_MfinActEn	(1<<4)
  #define MFRC522_CRCIEn	(1<<2)
#define MFRC522_REG_ComIrq	0x04 /* interrupt request bits */
  #define MFRC522_Set1		(1<<7)
  #define MFRC522_TxIRQ		(1<<6)
  #define MFRC522_RxIRQ		(1<<5)
  #define MFRC522_IdleIRq	(1<<4)
  #define MFRC522_HiAlertIRq	(1<<3)
  #define MFRC522_LoAlertIrq	(1<<2)
  #define MFRC522_ErrIRq	(1<<1)
  #define MFRC522_TimerIRq	(1<<0)
#define MFRC522_REG_DivIrq	0x05 /* interrupt request bits */
  #define MFRC522_Set2		(1<<7)
  #define MFRC522_MfinActIRq	(1<<4)
  #define MFRC522_CRCIRq	(1<<2)
#define MFRC522_REG_Error		0x06 /* error bits showing the error status of the last command executed */
  #define MFRC522_WrErr		(1<<7)
  #define MFRC522_TempErr	(1<<6)
  #define MFRC522_BufferOvfl	(1<<4)
  #define MFRC522_CollErr	(1<<3)
  #define MFRC522_CRCErr	(1<<2)
  #define MFRC522_ParityErr	(1<<1)
  #define MFRC522_ProtocolErr	(1<<0)
#define MFRC522_REG_Status1	0x07 /* communication status bits */
  #define MFRC522_CRCOk		(1<<6)
  #define MFRC522_CRCReady	(1<<5)
  #define MFRC522_IRq		(1<<4)
  #define TRunning		(1<<3)
  #define MFRC522_HiAlert	(1<<1)
  #define MFRC522_LoAlert	(1<<0)
#define MFRC522_REG_Status2	0x08 /* receiver and transmitter status bits */
  #define MFRC522_TempSensClear	(1<<7)
  #define MFRC522_I2CForceHS	(1<<6)
  #define MFRC522_MFCrypto1On	(1<<3)
  #define MFRC522_ModemState	0 /* 3 bits */

#define MFRC522_REG_FIFOData	0x09 /* input and output of 64 byte FIFO bufffer */
  #define MFRC522_FIFOData	0 /* 8 bits */
#define MFRC522_REG_FIFOLevel	0x0a /* number of bytes stored in the FIFO buffer */
  #define MFRC522_FlushBuffer	(1<<7)
  #define MFRC522_FIFOLevel	0 /* 7 bits */
#define MFRC522_REG_WaterLevel	0x0b /* level for FIFO underflow and overflow warning */
  #define MFRC522_WaterLevel	0 /* 6 bits */
#define MFRC522_REG_Control	0x0c /* miscellaneous control registers */
  #define MFRC522_TStopNow	(1<<7)
  #define MFRC522_TStartNow	(1<<6)
  #define MFRC522_RxLastBits	(1<<0) /* 3 bits */
#define MFRC522_REG_BitFraming	0X0d /* adjustments for bit-oriented frames */
  #define MFRC522_StartSend	(1<<7)
  #define MFRC522_RxAlign	4 /* 3 bits */
  #define MFRC522_TxLastBits	0 /* 3 bits */
#define MFRC522_REG_Coll		0x0e /* bit position of the first bit-collision detected on the RF interface */
  #define MFRC522_ValuesAfterColl (1<<7)
  #define MFRC522_CollPosNotValid (1<<5)
#define MFRC522_REG_Reserved2	0x0f

/* Command */
#define MFRC522_REG_Reserved3	0x10
#define MFRC522_REG_Mode		0x11 /* defines general modes for transmitting and receivig */
  #define MFRC522_MSBFirst	(1<<7)
  #define MFRC522_TxWaitRF	(1<<5)
  #define MFRC522_PolMFin	(1<<3)
  #define MFRC522_CRCPreset	0 /* 2 bits */
#define MFRC522_REG_TxMode		0x12 /* defines transmission data and framing */
  #define MFRC522_TxCRCEn	(1<<7)
  #define MFRC522_TxSpeed	4 /* 3 bits */
  #define MFRC522_InvMod	(1<<3)
#define MFRC522_REG_RxMode		0x13 /* defineds reception data rate and framinng */
  #define MFRC522_RxCRCEn	(1<<7)
  #define MFRC522_Rxspeed	4 /* 3 bits */
  #define MFRC522_RxNoErr	(1<<3)
  #define MFRC522_RxMultiple	(1<<2)
#define MFRC522_REG_TxControl	0x14 /* controls the logical behavior of the antenna driver pins TX1 and TX2 */
  #define MFRC522_InvTx2RFOn	(1<<7)
  #define MFRC522_InvTx1RFOn	(1<<6)
  #define MFRC522_InvTx2RFOff	(1<<5)
  #define MFRC522_InvTx1RFOff	(1<<4)
  #define MFRC522_Tx2CW		(1<<3)
  #define MFRC522_Tx2RFEn	(1<<1)
  #define MFRC522_Tx1RFEn	(1<<0)
#define MFRC522_REG_TxAuto		0x15 /* control the setting of the transmission modulation */
#define MFRC522_Force100ASK	(1<<6)
#define MFRC522_REG_TxSell		0x16 /* selects the intermal sources for the antenna driver */
  #define MFRC522_DriverSel	4 /* 2 bits */
  #define MFRC522_MFOutSel	0 /* 3 bits */
#define MFRC522_REG_RxSel		0x17 /* selects internal recevier settings */
  #define MFRC522_UARTSel	6 /* 2 bits */
  #define MFRC522_RxWait	0 /* 6 bits */
#define MFRC522_REG_RxThreshold	0x18 /* selects thresholds for the bit decoder */
  #define MFRC522_MinLevel	4 /* 4 bits */
  #define MFRC522_CollLevel	0 /* 2 bits */
#define MFRC522_REG_Demod		0x19
  #define MFRC522_AddIQ		6 /* 2 bits */
  #define MFRC522_FixIQ		(1<<5)
  #define MFRC522_TPrescalEven	(1<<4)
  #define MFRC522_TauRcv	2 /* 2 bits */
  #define MFRC522_TauSync	0 /* 2 bits */
#define MFRC522_REG_Reserved4	0x1a
#define MFRC522_REG_Reserved5	0x1b
#define MFRC522_REG_MfTx		0x1c /* control some MIFARE connunication transmit parameters */
#define MFRC522_REG_MfRx		0x1d /* control some MIFARE communication receive parameters */
#define MFRC522_REG_Reserved6	0x1e
#define MFRC522_REG_SerialSpeed	0x1f /* selects the speed of the serial UART interface */
/* Configuration */
#define MFRC522_REG_Reserved7	0x20
#define MFRC522_REG_CRCResult_H	0x21 /* shows the MSB ad LSB values of the CRC calculation */
#define MFRC522_REG_CRCResult_L	0x22
#define MFRC522_REG_Reserved8	0x23
#define MFRC522_REG_ModWidth	0X24 /* controls the ModWIdth setting */
#define MFRC522_REG_Reserved9	0x25
#define MFRC522_REG_RFCfg		0x26 /* configures the receiver gain */
#define MFRC522_REG_GsN		0x27 /* selects the conductance of the antenna driver pins TX1 and TX2 for modulation */
#define MFRC522_REG_CWGs		0x28 /* defines the conductance of the p-driver output during periods of no modulation */
#define MFRC522_REG_ModGs		0x29 /* defines the conductance of the p-driver output during perods of modulation */
#define MFRC522_REG_TMode		0x2a /* deffines settings for the internal timer */
#define MFRC522_REG_TPrescaler	0x2b
#define MFRC522_REG_TReload_H	0x2c /* defines thee 16-bit timer reload value */
#define MFRC522_REG_TReload_L	0x2d
#define MFRC522_REG_TCounterVal_H	0x2e /* shows the 16-bit timer value */
#define MFRC522_REG_TCounterVal_L	0x2f 
/* Test register */
#define MFRC522_REG_RESERVED10	0x30
#define MFRC522_REG_TestSel1	0x31 /* general test signal configuration */
#define MFRC522_REG_TestSel2	0x32 /* general test signal configuration and PRBS control */
#define MFRC522_REG_TestPinEn	0x33 /* enables pin output driver on pins D1 to D7 */
#define MFRC522_REG_TestPinValue	0x34 /* defines the values fot D1 to D7 when it is used as an I/O bus */
#define MFRC522_REG_TestBus	0x35 /* shows the status of the internal test bus */
#define MFRC522_REG_AutoTest	0x36 /* controls the digital self test */
#define MFRC522_REG_Version	0x37 /* shows the software version */
#define MFRC522_REG_AnalogTest	0x38 /* controls the pins AUX1 and AUX2 */
#define MFRC522_REG_TestDAC1	0x39 /* defines the test value for TestDAC1 */
#define MFRC522_REG_TestDAC2	0x3a /* defines the test value for TestDAC1 */
#define MFRC522_REG_TestADC	0x3b /* shows the value of ADC I and Q channels */
// 0x3c t0 0x3f Reserved
/*@}*/

/*@{*/
/** Command */
#define MFRC522_CMD_Idle		0x00
#define MFRC522_CMD_CalcCRC		0x03
#define MFRC522_CMD_Transmit		0x04
#define MFRC522_CMD_NoCmdChange		0x07
#define MFRC522_CMD_Recevive		0x08
#define MFRC522_CMD_Transceive		0x0c
#define MFRC522_CMD_MFAuthent		0x0e
#define MFRC522_CMD_SoftReset		0x0f
/*@}*/


/**
 * @brief Initialize mfrc522
 */
void mfrc522_init( void );

#define ISO14443_TYPE_A	0
uint8_t mfrc522_setISOType( uint8_t type );

/* Turn on or off the anntenna.
 *
 * @para on If on is nonzero, turn on the anntenna,
 * or turn off.
 */
void mfrc522_setAntennaPower( uint8_t on );

/* timer operations */
void mfrc522_timer_setCount( uint16_t cnt ); /* cnt * 100us */
void mfrc522_timer_start( );
void mfrc522_timer_stop();

/* calculate CRC */
#define MFRC522_CRC_PRESET_0000 0x00
#define MFRC522_CRC_PRESET_6363	0x01
#define MFRC522_CRC_PRESET_A671	0x02
#define MFRC522_CRC_PRESET_FFFF	0x03
void mfrc522_crc_setPreset( uint8_t preset );
void mfrc522_crc_startCalc( void );
void mfrc522_crc_calcData( uint8_t data );
void mfrc522_crc_calcArray( uint8_t *data, uint8_t len );
void mfrc522_crc_stop( void );
uint16_t mfrc522_crc_getResult( void );

/* transcceie */
typedef struct mfrc522_TransceiveData{
	uint8_t *data_send;
	uint8_t data_send_len;
	uint8_t *data_recv;
	uint8_t data_recv_len;
	uint8_t last_bits;
} mfrc522_transceive_data_t;
#define MFRC522_RET_CODE_OK	0
#define MFRC522_RET_CODE_NOTAG	1
#define MFRC522_RET_CODE_ERROR	3
/**
 * \brief Transceive command
 *
 * \para data Inclue the data to send and the data to receive.
 * \return MFRC522_RET_CODE_OK It is OK
 * \return MFRC522_RET_CODE_NOTAG Not card to communicate.
 * \return MFRC522_RET_CODE_ERROR Some error
 */
uint8_t mfrc522_transceive( mfrc522_transceive_data_t *data );



/* commmand */
void mfrc522_cmd_idle();
void mfrc522_cmd_calcCRC();
void mfrc522_cmd_transmit();
void mfrc522_cmd_noCmdChange();
void mfrc522_cmd_transceive();
void mfrc522_cmd_mfAuthent();
void mfrc522_cmd_softReset();


void mfrc522_test();
void mfrc522_timer_test();

/* card operation */
uint8_t card_request( uint8_t req_code, uint16_t *tag_type );

/* pin operations */
extern void mfrc522_pin_init( void );
extern void mfrc522_pin_NSS_set( uint8_t v );
extern void mfrc522_pin_SCLK_set( uint8_t v );
extern void mfrc522_pin_MOSI_set( uint8_t v );
extern uint8_t mfrc522_pin_MISO_get( void );
extern void mfrc522_pin_RST_set( uint8_t v );

#endif /* _MFRC522_H_ */
