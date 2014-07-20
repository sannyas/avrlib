TARGET_NAME = bmp085_demo
TARGET_TYPE = prj

PRJ_CSRC = main.c
PRJ_MODULES = pcf8591 avr/i2c avr/usart

export F_CPU = 8000000
export DEVICE = atmega16

export TWI_FREQUENCY = 100000

export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
