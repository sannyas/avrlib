/**
 * @file hmc5883l.h
 * @brief hmc5883l sendor module.
 * author: Liao MY
 * data: 2013/11/19
 *
 * usage:
 * 	+ The first step is calling function `hmc5883l_init()`
 * 		This file request a parameter conf whose type is 
 * 		hmc5883l_configuration_t to initialize this
 * 		device. If parameter conf is NULL, this will be
 * 		initialized using default configuration( see
 * 		datasheet ).
 *
 * 		After initialization. This device alse can be 
 * 		configurated with calling function
 * 		hmc5883l_writeConfiguration().
 * 		the type hmc5883l_configuration_t is consist of
 * 		conf_a, conf_b and mode. These corresponds to
 * 		the register in hmc5883l chip. The sample 
 * 		code( as default ):
 *
 * 		...
 * 		hmc5883l_configuration_t conf;
 * 		conf.conf_a =  HMC5883L_NUM_OF_SAMPLES_1 +
 * 				HMC5883L_DATA_OUTPUT_RATE_15 +
 * 				HMC5883L_MEASUREMENT_MODE_NORMAL;
 * 		conf.conf_b = HMC5883L_GAIN_SETTING_1090;
 * 		conf.mode = HMC5883L_OPERATING_MODE_SIGNAL;
 * 		hmc5883l_writeConfiguration( &conf );
 * 		...
 *
 * 	+ The next step is reading data;
 * 		The sample code:
 *
 * 		...
 * 		hmc5883l_sensor_data_t sdata;
 * 		hmc5883l_getSensorData( &sdata );
 * 		sdata.x OR sdata.y OR sdata.z
 * 		...
 */

#ifndef _HMC5883L_H_
#define _HMC5883L_H_


#include <stdint.h>

/** 
 * @name Reigsters
 * Registers define and description
 *
 * @{
 */

/**
 * @brief Configuration register A
 *
 * The Configuration register is used to config
 * the device for setting the data output rate
 * and measurement configuration
 */
#define HMC5883L_REG_ADDR_CONF_A	0

  /**
   * Select number of samples averaged (1 to 8)
   * per measurement output.
   */
  #define HMC5883L_NUM_OF_SAMPLES_1	0<<5	/* default */
  #define HMC5883L_NUM_OF_SAMPLES_2	1<<5
  #define HMC5883L_NUM_OF_SAMPLES_4	2<<5
  #define HMC5883L_NUM_OF_SAMPLES_8	3<<5

  /** Data Output Rate Bits. These bits set the rate at which
   * data is written to all three data output registers.
   */
  #define HMC5883L_DATA_OUTPUT_RATE_D75	0<<2	/* 0.75 Hz */
  #define HMC5883L_DATA_OUTPUT_RATE_1D5	1<<2	/* 1.5 Hz */
  #define HMC5883L_DATA_OUTPUT_RATE_3	2<<2	/* 3 Hz */
  #define HMC5883L_DATA_OUTPUT_RATE_7D5	3<<2	/* 7.5 Hz */
  #define HMC5883L_DATA_OUTPUT_RATE_15	4<<2	/* 15 Hz, default */
  #define HMC5883L_DATA_OUTPUT_RATE_30	5<<2	/* 30 Hz */
  #define HMC5883L_DATA_OUTPUT_RATE_75	6<<2	/* 75 Hz */

  /** Measurement Configuration Bits. These bits define the
   * measurement flow of the device, specifically whether
   * or not to incorporate an applied bias into the measurement.
   */
  #define HMC5883L_MEASUREMENT_MODE_NORMAL		0<<0	/* defauult */
  #define HMC5883L_MEASUREMENT_MODE_POSITIVE_BIAS	1<<0
  #define HMC5883L_MEASUREMENT_MODE_NEGATIVE_BIAS	2<<0
  
/** @brief The Configuration register B for setting the
 * device gain.
 */
#define HMC5883L_REG_ADDR_CONF_B	1<<5

  /** Gain settingg, LSb/Gauss */
  #define HMC5883L_GAIN_SETTING_1370	0<<5	/* +- 0.8 Ga */
  #define HMC5883L_GAIN_SETTING_1090	1<<5	/* +- 1.3 Ga, default */
  #define HMC5883L_GAIN_SETTING_820	2<<5	/* +- 1.9 Ga */
  #define HMC5883L_GAIN_SETTING_660	3<<5	/* +- 2.5 Ga */
  #define HMC5883L_GAIN_SETTING_440	4<<5	/* +- 4.0 Ga */
  #define HMC5883L_GAIN_SETTING_390	5<<5	/* +- 4.7 Ga */
  #define HMC5883L_GAIN_SETTING_330	6<<5	/* +- 5.6 Ga */
  #define HMC5883L_GAIN_SETTING_230	7<<5	/* +- 8.1 Ga */

/* @brief Mode register
 *
 * This register is used to select the operating mode of the device.
 */
#define HMC5883L_REG_ADDR_MODE	2
  #define HMC5883L_OPERATING_MODE_CONTINUOUS	0<<0
  #define HMC5883L_OPERATING_MODE_SIGNAL	1<<0
  #define HMC5883L_OPERATING_MODE_IDLE		2<<0

/** The data output X registers */
#define HMC5883L_REG_ADDR_X_MSB	3
#define HMC5883L_REG_ADDR_X_LSB	4

/** The data output Z registers */
#define HMC5883L_REG_ADDR_Z_MSB	5
#define HMC5883L_REG_ADDR_Z_LSB	6

/** The data output X registers */
#define HMC5883L_REG_ADDR_Y_MSB	7
#define HMC5883L_REG_ADDR_Y_LSB	8

/** Status Register */
#define HMC5883L_REG_ADDR_STATUS	9

/** 
 * @brief Identification Register A(B,C)
 *
 * The identification register A(B,C) is used to identiry the device.
 */
#define HMC5883L_REG_ADDR_IDENTIFCATION_A	10
#define HMC5883L_REG_ADDR_IDENTIFCATION_B	11
#define HMC5883L_REG_ADDR_IDENTIFCATION_C	12

/*@}*/

/** 
 * @name I2C slaver address
 *
 *@{
 */
#define HMC5883L_SLAVER		0x1e
#define HMC5883L_SLAVER_READ	0x3d
#define HMC5883L_SLAVER_WRITE	0x3c
/*@}*/

/**
 * @brief The sensor data struct.
 */
typedef struct hmc5883l_SensorData{
	int16_t x; /**< The value of x axis */
	int16_t z; /**< The value of y axis */
	int16_t y; /**< The value of z axis */
} hmc5883l_sensor_data_t;

/**
 * @brief The configuration struct.
 *
 * This struct used to configure this chip. All fields
 * in struct correspond to related register.
 */
typedef struct hmc5883l_Configuration{
	uint8_t conf_a; /**< Configuration register A */
	uint8_t conf_b; /**< Configuration register B */
	uint8_t mode;	/**< Mode register */
} hmc5883l_configuration_t;


/** 
 * @brief hmc5883 intialization
 *
 * Must call this function before using this device.
 */
void hmc5883l_init( hmc5883l_configuration_t *conf );

/**
 * @name configure
 * Read and write configuration.
 *
 * @{
 */
void hmc5883l_readConfiguration( hmc5883l_configuration_t *conf );
void hmc5883l_writeConfiguration( hmc5883l_configuration_t *conf );
/*@}*/

/** 
 * @brief Read data from hmc5883 chip.
 *
 * @param data [out] The sensor data will be write
 * to parameter data
 */
void hmc5883l_getSensorData( hmc5883l_sensor_data_t *data );


uint8_t hmc5883l_selfTest( void );
/* TODO */


#endif

