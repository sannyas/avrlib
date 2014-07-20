TARGET_NAME = hmc588l_demo
TARGET_TYPE = prj

PRJ_CSRC = main.c
PRJ_ASRC = 
PRJ_MODULES = hmc5883l avr/usart avr/i2c avr/twi
#PRJ_DEFS = HMC5883L_DEBUG 

export DEVICE = atmega16
export F_CPU = 8000000

export USART_BAUD = 9600
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE
export TWI_FREQUENCY = 100000

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
