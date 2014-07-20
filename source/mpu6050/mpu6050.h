/**
 * @file mpu6050.h
 * @brief mpu6050
 * @author Liao MY
 * @date 2014-07-15
 */

/* Copyright (C) 
 * 2014 - Liao MY
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


#ifndef _H_MPU6050_
#define _H_MPU6050_

#include <stdint.h>
#include "mpu6050_reg.h"

/*-----------------------------------------------------------*/
/* Set Clock source
 *-----------------------------------------------------------*/
/**
 * @name clock_source
 * @{ */
#define CLOCK_INTERNAL_8MHz			0
#define CLOCK_PLL_XGYRO				1
#define CLOCK_PLL_YGYRO				2
#define CLOCK_PLL_ZGYRO				3
#define CLOCK_PLL_EXTERNAL_32KHz		4
#define CLOCK_PLL_EXTERNAL_19MHz		5
#define CLOCK_STOP				7

/**
 * @brief Set clock source.
 * @param clksel Clcok source, this value should be CLOCK_*.
 */
void 	mpu6050_setClockSource( uint8_t clksel );

/**
 * @brief Get clock sourcce.
 * @return Curent clock sourcce.
 */
uint8_t mpu6050_getClockSource( void );

/**  @} */

/*-----------------------------------------------------------*/
/* Set sample rate divider.
 *-----------------------------------------------------------*/
/**
 * @brief Set sample rate divider.
 * @param div divider, simple rate = gyro output rate / (1+div)
 */
void mpu6050_setSampleRateDivider( uint8_t div );


/*-----------------------------------------------------------*/
/* Set sleep mode
 *-----------------------------------------------------------*/
/**
 * @name low_pass_filter
 * @{ */
#define DLPF_256Hz	0
#define DLPF_188Hz	1
#define DLPF_98Hz	2
#define DLPF_42Hz	3
#define DLPF_20Hz	4
#define DLPF_10Hz	5
#define DLPF_5Hz	6

/**
 * @brief Set digital low pass filter.
 * @param filter this value must be DLPF_*
 */
void mpu6050_setLowPassFilter( uint8_t filter );

/**  @} */


/*-----------------------------------------------------------*/
/* Set sleep mode
 *-----------------------------------------------------------*/
/**
 * @brief Enabe or disable sleep mode.
 * @param enable 1 or 0.
 */
void mpu6050_sleep( uint8_t enable );


/*-----------------------------------------------------------*/
/* Set full scale of gyroscope.
 *-----------------------------------------------------------*/
/**
 * @name gyroscope_full_scale
 * @{ */

#define GYRO_FULLSCALE_250	(0<<MPU6050_BIT_FS_SEL)
#define GYRO_FULLSCALE_500	(1<<MPU6050_BIT_FS_SEL)
#define GYRO_FULLSCALE_1000	(2<<MPU6050_BIT_FS_SEL)
#define GYRO_FULLSCALE_2000	(3<<MPU6050_BIT_FS_SEL)

/**
 * @brief Select the full scale range of gyroscope outputs.
 * @param scale This value should be GYRO_FULLSCALE_*.
 */
void mpu6050_setGyroFullScale( uint8_t scale );

/**
 * @brief Get the full scale range of gyroscope outputs.
 * @return GYRO_FULLSCALE_*
 */
uint8_t mpu6050_getGyroFullScale( void );

/**  @} */


/*-----------------------------------------------------------*/
/* Set full scale of accelerometer.
 *-----------------------------------------------------------*/
/**
 * @name accel_full_scale
 * @{ */

#define ACCEL_FULLSCALE_2G	(0<<MPU6050_BIT_AFS_SEL)
#define ACCEL_FULLSCALE_4G	(1<<MPU6050_BIT_AFS_SEL)
#define ACCEL_FULLSCALE_8G	(2<<MPU6050_BIT_AFS_SEL)
#define ACCEL_FULLSCALE_16G	(3<<MPU6050_BIT_AFS_SEL)
/* Select the full scale of the accelerometer outputs */

/**
 * @brief Set the full scale range of accelerometer outputs.
 * @param scale This value should be ACCEL_FULLSCALE_*.
 */
void mpu6050_setAccelFullScale( uint8_t scale );


/**
 * @brief Set the full range of accelerometer outputs.
 * @return ACCEL_FULLSCALE_*
 */
uint8_t mpu6050_getAccelFullScale( void );

/**  @} */


/*-----------------------------------------------------------*/
/* Self test.
 *-----------------------------------------------------------*/
/**
 * @name self_test
 * @{
 */

/**
 * @brief Gyroscope self test
 * @return If test OK, return 1, or return 0.
 */
uint8_t mpu6050_gyroSelfTest( void );

/**
 * @brief Accelerometer self test
 * @return If test OK, return 1, or return 0.
 */
uint8_t mpu6050_accelSelfTest( void );
 
/** @} */
 

/*-----------------------------------------------------------*/
/* Get sensor data.
 *-----------------------------------------------------------*/

typedef struct mpu6050_SensorData{
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float accel_x;
	float accel_y;
	float accel_z;
} mpu6050_sensor_data_t;


/**
 * @brief Get sensor data.
 * @param data The sstrut where the new sensor data will wirte to.
 */
void mpu6050_getSendorData( mpu6050_sensor_data_t *data );

 
void	mpu6050_init( void );

/* extern functions */
void	mpu6050_platformInit( void );
uint8_t mpu6050_i2cRead( uint8_t reg, uint8_t *data, uint8_t len );
uint8_t mpu6050_i2cWrite( uint8_t reg, uint8_t *data, uint8_t len );
void	mpu6050_delay_ms( uint16_t n );

void mpu6050_test( void );

#endif
