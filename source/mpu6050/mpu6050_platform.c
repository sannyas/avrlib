#include <stdio.h>

#include "stm32f10x.h"                  // Device header
#include "mpu6050.h"
#include "../serial/serial.h"
#include "../i2c/i2c.h"

#define MPU6050_SLAVE_ADDR	(0x68)

void mpu6050_platformInit( void ){
}

uint8_t mpu6050_i2cRead( uint8_t reg, uint8_t *data, uint8_t len ){
	return 1;
}

uint8_t mpu6050_i2cWrite( uint8_t reg, uint8_t *data, uint8_t len ){
	return 1;
}

void mpu6050_delay_ms( uint16_t n ){
}
