TARGET_NAME = pdiusbd12
TARGET_TYPE = prj

PRJ_CSRC = pdiusbd12.c pdiusbd12_pin.c main.c pdiusbd12_usb.c user.c
PRJ_ASRC = 
PRJ_MODULES = avr/usart
PRJ_DEFS = PDIUSBD12_DEBUG

export DEVICE = atmega16
export F_CPU = 8000000
export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
