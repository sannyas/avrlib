#include <stdio.h>
#include "bmp085.h"

/* 
 * 平台依赖的函数或宏
 */
#include <i2c.h>
void bmp085_readData( uint8_t addr, uint8_t *data, uint8_t len ){
	i2c_readBytes( BMP085_I2C_ADDR, addr, data, len );
}

void bmp085_writeData( uint8_t addr, uint8_t *data, uint8_t len ){
	i2c_writeBytes( BMP085_I2C_ADDR, addr, data, len );
}

#include <util/delay.h>
#define bmp085_delay_ms(n)	_delay_ms(n)


/*
 * Static data
 */
static bmp085_calibration_coef_t cal_coef;
#define cc cal_coef

/*
 * Static function
 */
static void bmp085_test_displayCalibration( void );
static int16_t bmp085_readInt16( uint8_t addr );

/*****************************************************/
void bmp085_init( void ){
	bmp085_getCalibrationCoef( &cc );
	#ifdef BMP085_DEBUG
	bmp085_test_displayCalibration();
	#endif
}


static void __swap_uint16( uint8_t *data ){
	uint8_t temp;
	temp = data[0];
	data[0] = data[1];
	data[1] = temp;
}

/*****************************************************/
void bmp085_getCalibrationCoef( bmp085_calibration_coef_t *cal_p ){
#if 0
	uint8_t i=0;
	
	bmp085_readData( 
		BMP085_CC_ADDR_START,
		(uint8_t*)cal_p, 
		sizeof(bmp085_calibration_coef_t ) 
		);
	for( i=0; i<11; ++i ){
		__swap_uint16( (uint8_t*)cal_p + i*2 );
	}

	#ifdef BMP085_DEBUG
	bmp085_test_displayCalibration();
	#endif
#endif
	cal_p->AC1 = bmp085_readInt16(0xAA);
	cal_p->AC2 = bmp085_readInt16(0xAC);
	cal_p->AC3 = bmp085_readInt16(0xAE);
	cal_p->AC4 = bmp085_readInt16(0xB0);
	cal_p->AC5 = bmp085_readInt16(0xB2);
	cal_p->AC6 = bmp085_readInt16(0xB4);
	cal_p->B1 =  bmp085_readInt16(0xB6);
	cal_p->B2 =  bmp085_readInt16(0xB8);
	cal_p->MB =  bmp085_readInt16(0xBA);
	cal_p->MC =  bmp085_readInt16(0xBC);
	cal_p->MD =  bmp085_readInt16(0xBE);
}

/*****************************************************/
uint16_t bmp085_getUT( void ){
	uint16_t data16;
	uint8_t data8 = BMP085_MEASURE_TEMPERATURE;

	bmp085_writeData( 
		BMP085_RA_CONTROL,
		&data8,
		1
		 );
	bmp085_delay_ms( 5 );
#if 0
	bmp085_readData( 
		BMP085_RA_MSB,
		(uint8_t*)&data16,
		2
		);
	__swap_uint16( (uint8_t*)&data16 );
#endif
	data16 = bmp085_readInt16( BMP085_RA_MSB );

	return data16;
}

/*****************************************************/
int32_t bmp085_getUP( uint8_t oss ){

	uint8_t temp = 0x34 + ( oss << 6 ); /* the value of control reg. */
	uint8_t data[3];
	uint32_t up;
	
	bmp085_writeData( 
		BMP085_RA_CONTROL,
		&temp,
		1
		);
		
	if( oss == 0 )
		bmp085_delay_ms( 5 );
	else if( oss == 1 )
		bmp085_delay_ms( 8 );
	else if( oss == 2 )
		bmp085_delay_ms( 14 );
	else if( oss == 3 )
		bmp085_delay_ms( 26 );
	// bmp085_delay_ms( 2+(3<<oss) );

	bmp085_readData( 
		BMP085_RA_MSB,
		data,
		3
		);


	up = ((uint32_t)(data[0])<<16)+((uint32_t)(data[1])<<8)+(uint32_t)(data[2]);
	up >>= ( 8-oss );
	return up;
}

/*****************************************************/
void bmp085_getADTData( bmp085_adt_data_t *data, uint8_t oss ){
	data->ut = bmp085_getUT();
	data->up = bmp085_getUP( oss );
}
 

/*****************************************************/
void bmp085_getSensorData( bmp085_sensor_data_t *tdata, uint8_t oss ){
	int32_t  x1, x2, x3;
	int32_t  b3, b5, b6;
	uint32_t b4, b7;
	int32_t  p;
	int32_t  ut = bmp085_getUT( );
	int32_t  up = bmp085_getUP( oss );

	x1 = (((int32_t)ut - (int32_t)cc.AC6)*(int32_t)cc.AC5) >> 15;
	x2 = ((int32_t)cc.MC << 11)/(x1 + cc.MD);
	b5 = x1 + x2;

	tdata->temperature = ((b5 + 8)>>4);  

	b6 = b5 - 4000;
	// Calculate B3
	x1 = (cc.B2 * (b6 * b6)>>12)>>11;
	x2 = (cc.AC2 * b6)>>11;
	x3 = x1 + x2;
	b3 = (((((int32_t)cc.AC1)*4 + x3)<<oss) + 2)>>2;

	// Calculate B4
	x1 = (cc.AC3 * b6)>>13;
	x2 = (cc.B1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	b4 = (cc.AC4 * (uint32_t)(x3 + 32768L))>>15;

	b7 = ((uint32_t)(up - b3) * (50000L>>oss));
	if (b7 < 0x80000000UL)
		p = (b7<<1)/b4;
	else
		p = (b7/b4)<<1;

	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	p += (x1 + x2 + 3791L)>>4;

	tdata->pressure = p;
}

/*****************************************************/
#if 0
#include <math.h>
int32_t bmp085_getAltitude( int32_t p ){
	int altitude;
	altitude = 44330*(1-pow((p/101325.0),0.190295));
	return altitude;
}
#endif

/*****************************************************/
static void bmp085_test_displayCalibration( void ){
	printf( "AC1 = %d\n", cal_coef.AC1 );
	printf( "AC2 = %d\n", cal_coef.AC2 );
	printf( "AC3 = %d\n", cal_coef.AC3 );
	printf( "AC4 = %u\n", cal_coef.AC4 );
	printf( "AC5 = %u\n", cal_coef.AC5 );
	printf( "AC6 = %u\n", cal_coef.AC6 );
	printf( "B1  = %d\n", cal_coef.B1  );
	printf( "B2  = %d\n", cal_coef.B2  );
	printf( "MB  = %d\n", cal_coef.MB  );
	printf( "MC  = %d\n", cal_coef.MC  );
	printf( "MD  = %d\n", cal_coef.MD  );
}

/*****************************************************/
static int16_t bmp085_readInt16( uint8_t addr ){
	uint8_t data[2];
	bmp085_readData( addr, data, 2 );
	return ((uint16_t)(data[0])<<8)+(uint16_t)(data[1]);
}
