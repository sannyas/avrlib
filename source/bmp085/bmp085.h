#ifndef _BMP085_H_
#define _BMP085_H_

#include <stdint.h>


/** @name I2C address of bmp085*/
/*@{*/
#define BMP085_I2C_ADDR		0x77
#define BMP085_I2C_ADDR_WRITE	0xee
#define BMP085_I2C_ADDR_READ	0xef
/*@}*/

/**
 * @name regs
 * Register address in bmp085. these address is
 * used to access with i2c communication.
 */
/*@{*/
#define BMP085_RA_AC1_H     0xAA    /* AC1_H */
#define BMP085_RA_AC1_L     0xAB    /* AC1_L */
#define BMP085_RA_AC2_H     0xAC    /* AC2_H */
#define BMP085_RA_AC2_L     0xAD    /* AC2_L */
#define BMP085_RA_AC3_H     0xAE    /* AC3_H */
#define BMP085_RA_AC3_L     0xAF    /* AC3_L */
#define BMP085_RA_AC4_H     0xB0    /* AC4_H */
#define BMP085_RA_AC4_L     0xB1    /* AC4_L */
#define BMP085_RA_AC5_H     0xB2    /* AC5_H */
#define BMP085_RA_AC5_L     0xB3    /* AC5_L */
#define BMP085_RA_AC6_H     0xB4    /* AC6_H */
#define BMP085_RA_AC6_L     0xB5    /* AC6_L */
#define BMP085_RA_B1_H      0xB6    /* B1_H */
#define BMP085_RA_B1_L      0xB7    /* B1_L */
#define BMP085_RA_B2_H      0xB8    /* B2_H */
#define BMP085_RA_B2_L      0xB9    /* B2_L */
#define BMP085_RA_MB_H      0xBA    /* MB_H */
#define BMP085_RA_MB_L      0xBB    /* MB_L */
#define BMP085_RA_MC_H      0xBC    /* MC_H */
#define BMP085_RA_MC_L      0xBD    /* MC_L */
#define BMP085_RA_MD_H      0xBE    /* MD_H */
#define BMP085_RA_MD_L      0xBF    /* MD_L */
#define BMP085_RA_CONTROL   0xF4    /* CONTROL */
#define BMP085_RA_MSB       0xF6    /* MSB */
#define BMP085_RA_LSB       0xF7    /* LSB */
#define BMP085_RA_XLSB      0xF8    /* XLSB */
#define BMP085_CC_ADDR_START	0xaa
#define BMP085_ADT_ADDR		0xf6
/*@}*/

/**
 * @name Measurement
 * These values is specific what to mesure 
 * when using i2c to communication with bmp085.
 */
/*@{*/
#define BMP085_MEASURE_TEMPERATURE	0X2e
#define BMP085_MEASURE_PRESSURE_0	0x34
#define BMP085_MEASURE_PRESSURE_1	0x74
#define BMP085_MEASURE_PRESSURE_2	0xb4
#define BMP085_MEASURE_PRESSURE_3	0xf4
/*@}*/

/** 
 * @brief Calibration coefficients
 *
 * This struct define the calibration coefficients, These
 * coefficients is chip dependent and used to calculate 
 * the true value( temperature and pressure ) from ADT data.
 */ 
typedef struct bmp085_CalibrationCoef{
	int16_t		AC1;
	int16_t 	AC2;
	int16_t 	AC3;
	uint16_t	AC4;
	uint16_t	AC5;
	uint16_t	AC6;
	int16_t		B1;
	int16_t		B2;
	int16_t		MB;
	int16_t		MC;
	int16_t		MD;
}bmp085_calibration_coef_t;

/**
 * @brief The adt data in the bmp085
 */
typedef struct bmp085_ADTData{
	int32_t	 up;
	uint16_t ut;
}bmp085_adt_data_t;

/** 
 * @brief The true data calculated from adt data.
 */
typedef struct bmp085_SensorData{
	int32_t pressure;
	int32_t temperature;
}bmp085_sensor_data_t;

/**
 * @brief Initialize this device.
 *
 * The main step in this function is call bmp085_getCalibrationCoef().
 */
void bmp085_init( void );

/**
 * @brief get Calibration coefficients
 *
 * Calibration coefficients is used to calculate the true data
 * from ADT data. These coefficients is depend on specific
 * chip. In general, this function should be called at the
 * first.
 */
void bmp085_getCalibrationCoef( bmp085_calibration_coef_t *cal_addr );

/**
 * @brief read uncompensated temperature value( from ADT )
 * @return 16-bit unsigned uncompensated temperature value
 */
uint16_t bmp085_getUT( );

/**
 * @brief read uncompensated pressure value( from ADT )
 * @param oss ( oversampling setting ) The value is 0,1,2 and 3
 * @return 32-bit uncompensated pressure value
 */
#define BMP085_MODE_ULTRA_LOW_POWER		0
#define BMP085_MODE_STANDARD			1
#define BMP085_MODE_HIGH_RESOLUTION		2
#define BMP085_MODE_ULTRA_HIGH_RESOLUTION	3
int32_t  bmp085_getUP( uint8_t oss );

/**
 * @brief read UT and UP from chip
 */
void bmp085_getADTData( bmp085_adt_data_t *data, uint8_t oss );


/**
 * @brief Calculate the true data from sensor data( ADT data ).
 * @para sdata sensor data.
 * @para tdata true data.
 */
void bmp085_getSensorData( bmp085_sensor_data_t *sdata, uint8_t oss );

/**
 * @brief Calculatte altitude.
 *
 * @parm p pressure.
 * @return altitude.
 */
int32_t bmp085_getAltitude( int32_t p );

#endif

