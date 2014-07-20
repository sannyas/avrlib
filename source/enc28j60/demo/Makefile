TARGET_NAME = enc_demo
TARGET_TYPE = prj

# for project
PRJ_CSRC = main.c
PRJ_MODULES = avr/usart enc28j60

PRJ_DEFS =

export DEVICE = atmega16
export F_CPU = 8000000

export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

export ENC28J60_PIN_CS  = PORT_B4
export ENC28J60_PIN_SI  = PORT_B5
export ENC28J60_PIN_SCK = PORT_B7
export ENC28J60_PIN_SO  = PORT_B6
export ENC28J60_PIN_RESET = PORT_B0

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
