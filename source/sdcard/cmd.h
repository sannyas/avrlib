#ifndef _H_SD_CMD_
#define _H_SD_CMD_

/*
 * CMD in SPI mode
 */
#define CMD0	0
#define CMD1	1
#define CMD5	5
#define CMD6	6
#define CMD8	8
#define CMD9	9
#define CMD10	10
#define CMD11	11
#define CMD12	12
#define CMD13	13
#define CMD16	16
#define CMD17	17
#define CMD18	18
#define CMD24	24
#define CMD25	25
#define CMD27	27
#define CMD28	28
#define CMD29	29
#define CMD30	30
#define CMD32	32
#define CMD33	33
#define CMD34	34
#define CMD35	35
#define CMD36	36
#define CMD37	37
#define CMD38	38
#define CMD42	42
#define CMD50	50
#define CMD52	52
#define CMD53	53
#define CMD55	55
#define CMD56	56
#define CMD57	57
#define CMD58	58
#define CMD59	59

#define ACMD13	13
#define ACMD22	22
#define ACMD23	23
#define ACMD41	41
#define ACMD42	42
#define ACMD51	51

/*
 * Detailed Command Description
 */
#define GO_IDLE_STATE		CMD0
  #define HCS			0x40000000

#define SWITCH_FUN		CMD6
#define SEND_IF_COND		CMD8
#define SEND_CSD		CMD9
#define SEND_CID		CMD10
#define STOP_TRANSMISSION	CMD12
#define SEND_STATUS		CMD13
#define SET_BLOCKLEN		CMD16
#define READ_SINGLE_BLOCK	CMD17	
#define READ_MULTIPLE_BLOCK	CMD18
#define WRITE_BLOCK		CMD24
#define WRITE_MULTIPLE_BLOCK	CMD25
#define PEOGRAM_CSD		CMD27
#define SET_WRITE_PROT		CMD28
#define CLR_WRITE_PROT		CMD29
#define SEND_WRITE_PROT		CMD30
#define ERASE_WR_BLK_START_ADDR	CMD32
#define ERASE_WR_BLK_END_ADDR	CMD33
#define ERASE			CMD38
#define LOCK_UNLOCK		CMD42
#define APP_CMD			CMD55
#define GEN_CMD			CMD56
#define READ_OCR		CMD58
#define CRC_ON_OFF		CMD59

#define SD_STATUS		ACMD13
#define SEND_NUM_WR_BLOCKS	ACMD22
#define SET_WR_BLK_ERASE_COUNT	ACMD23
#define SD_SEND_OP_COND		ACMD41
#define SET_CLR_CARD_DETECT	ACMD42
#define SEND_SCR		ACMD52



/************************************
 * Response for R1 in SPI mode
 ***********************************/
#define IN_IDLE_STATE		0x01
#define ERASE_RESET		0x02
#define ILLEGAL_COMMAND		0x04
#define COM_CRC_ERROR		0x08
#define ERASE_SEQUENCE_ERROR	0x10
#define ADDRESS_ERROR		0x20
#define PARAMETR_ERROR		0x40

/*
 * Bitmap for OCR register
 */
#define OCR_BUSY		0x80000000
#define OCR_BUSY_BYTE		0x80
#define OCR_CCS			0x40000000
#define OCR_CCS_BYTE		0x40
#define OCR_UHS2		0x20000000
#define OCR_UHS2_BYTE		0x20
#endif
