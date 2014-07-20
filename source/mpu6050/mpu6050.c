#include <stdio.h>
#include "mpu6050.h"
#include "mpu6050_reg.h"

#define DEBUG_OUTPUT( fmt, args... )	printf( fmt, ##args )

/*----------------------------------------------------*/
/* Low level functions
 *----------------------------------------------------*/
static uint8_t mpu6050_readReg( uint8_t reg ){
	uint8_t data;
	mpu6050_i2cRead( reg, &data, 1 );
	return data;
}

static void mpu6050_writeReg( uint8_t reg, uint8_t data ){
	mpu6050_i2cWrite( reg, &data, 1 );
}

static void mpu6050_writeBits( uint8_t reg, uint8_t mask, uint8_t value ){
	uint8_t data;
	data = mpu6050_readReg( reg );
	data &= ~(mask);
	data |= value;
	mpu6050_writeReg( reg, data );
}

static uint16_t mpu6050_readADC( uint8_t reg ){
	uint16_t data;

	data = mpu6050_readReg( reg ) << 8;
	data += mpu6050_readReg( reg+1 );
	return data;
}

/*----------------------------------------------------*/
/* Set clock source
 *----------------------------------------------------*/
void mpu6050_setClockSource( uint8_t clksel ){
	mpu6050_writeBits( 
				MPU6050_REG_PWR_MGMT_1, 
				MPU6050_BIT_CLKSEL_MSK, 
				clksel
			);
}


/*----------------------------------------------------*/
/* Set sample rate divider.
 *----------------------------------------------------*/
void mpu6050_setSampleRateDivider( uint8_t div ){
	mpu6050_writeReg( MPU6050_REG_SMPLRT_DIV, div );
}


/*----------------------------------------------------*/
/* Set digital low pass filter.
 *----------------------------------------------------*/
void mpu6050_setLowPassFilter( uint8_t filter ){
	mpu6050_writeBits( MPU6050_REG_CONFIG, MPU6050_BIT_DLPF_CFG_MSK, filter );
}


/*----------------------------------------------------*/
/* Sleep mode
 *----------------------------------------------------*/
void mpu6050_sleep( uint8_t enable ){
	enable = enable? 1:0;
	mpu6050_writeBits( 
				MPU6050_REG_PWR_MGMT_1, 
				(1<<MPU6050_BIT_SLEEP),
				(enable<<MPU6050_BIT_SLEEP) 
			);
}

/*----------------------------------------------------*/
/* Set full scale of gyroscope
 *----------------------------------------------------*/
void mpu6050_setGyroFullScale( uint8_t scale ){
	mpu6050_writeBits( 
				MPU6050_REG_GYRO_CONFIG,
				MPU6050_BIT_FS_SEL_MSK,
				scale 
			);
}

/*----------------------------------------------------*/
/* Set full scale of accelerometer
 *----------------------------------------------------*/
void mpu6050_setAccelFullScale( uint8_t scale ){
	mpu6050_writeBits( 
				MPU6050_REG_ACCEL_CONFIG,
				MPU6050_BIT_AFS_SEL_MSK,
				scale
			);			
}


/*----------------------------------------------------*/
/* Get sensor data.
 *----------------------------------------------------*/

int16_t mpu6050_getAccelX( void ){
	return mpu6050_readADC( MPU6050_REG_ACCEL_XOUT_H );
}

int16_t mpu6050_getAccelY( void ){
	return mpu6050_readADC( MPU6050_REG_ACCEL_YOUT_H );
}

int16_t mpu6050_getAccelZ( void ){
	return mpu6050_readADC( MPU6050_REG_ACCEL_ZOUT_H );
}

uint16_t mpu6050_getTemp( void ){
	return mpu6050_readADC( MPU6050_REG_TEMP_OUT_H );
}

int16_t mpu6050_getGyroX( void ){
	return mpu6050_readADC( MPU6050_REG_GYRO_XOUT_H );
}

int16_t mpu6050_getGyroY( void ){
	return mpu6050_readADC( MPU6050_REG_GYRO_YOUT_H );
}

int16_t mpu6050_getGyroZ( void ){
	return mpu6050_readADC( MPU6050_REG_GYRO_ZOUT_H );
}


/*----------------------------------------------------*/
/* Gyroscope self test
 *----------------------------------------------------*/
uint8_t mpu6050_gyroSelfTest( void ){
	/* TODO */
	return 1;
}


/*-----------------------------------------------------------*/
/* Get sensor data.
 *-----------------------------------------------------------*/
void mpu6050_getSendorData( mpu6050_sensor_data_t *data ){
	data->gyro_x = mpu6050_getGyroX()/131.0*3.14159/180.0;
	data->gyro_y = mpu6050_getGyroY()/131.0*3.14159/180.0;
	data->gyro_z = mpu6050_getGyroZ()/131.0*3.14159/180.0;
	data->accel_x = mpu6050_getAccelX()/16384.0;
	data->accel_y = mpu6050_getAccelY()/16384.0;
	data->accel_z = mpu6050_getAccelZ()/16384.0;
}


/*----------------------------------------------------*/
/* Initialize
 *----------------------------------------------------*/
void mpu6050_init( void ){
	mpu6050_platformInit();

	mpu6050_setClockSource( CLOCK_PLL_XGYRO );
	mpu6050_setGyroFullScale( GYRO_FULLSCALE_250 );
	mpu6050_setAccelFullScale( ACCEL_FULLSCALE_2G );
	mpu6050_setLowPassFilter( DLPF_5Hz );
	mpu6050_sleep( 0 );
}


/*----------------------------------------------------*/
/* For test
 *----------------------------------------------------*/
void mpu6050_test( void ){
	mpu6050_init();

	while( 1 ){
		printf( "%d,%d,%d\r\n", 
			mpu6050_getAccelX(),
			mpu6050_getAccelY(),
			mpu6050_getAccelZ()
			);
		#if 0
		printf( "%d,%d,%d,",
			mpu6050_getGyroX(),
			mpu6050_getGyroY(),
			mpu6050_getGyroZ()
			);
		printf( "%u\r\n", mpu6050_getTemp() );
		#endif
		mpu6050_delay_ms( 10 );		
	}
#if 0
	uint8_t val;
	int i;
	mpu6050_platformInit();
	for( i=0; i<20; ++i ){
		val = mpu6050_readReg( i );
		printf( "----------- 0x%x\r\n", val );
	}
	printf( "who am i: 0x%x\t\n", mpu6050_readReg( MPU6050_REG_WHO_AM_I ) );
#endif
	
}

