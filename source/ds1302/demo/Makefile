TARGET_NAME = ds1302_demo
TARGET_TYPE = prj

PRJ_CSRC = main.c
PRJ_ASRC =
PRJ_MODULES = ds1302 avr/usart
#PRJ_DEFS = DS1302_DEBUG

export DEVICE = atmega16
export F_CPU = 8000000

export USART_BAUD = 9600
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

export DS1302_PIN_IO = D7
export DS1302_PIN_RST = D4
export DS1302_PIN_SCLK = D6

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
