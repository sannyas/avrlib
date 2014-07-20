TARGET_NAME = sdcard_demo
TARGET_TYPE = prj

# for project
PRJ_CSRC = sdcard.c main.c sdcard_pin.c
PRJ_ASRC = 
PRJ_MODULES = avr/usart
PRJ_DEFS = SDCARD_DEBUG

export DEVICE = atmega16
export F_CPU = 8000000
export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
