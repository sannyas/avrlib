TARGET_NAME = mfrc522_demo
TARGET_TYPE = prj

PRJ_CSRC = main.c
PRJ_MODULES = mfrc522 avr/usart

export DEVICE = atmega16
export F_CPU = 8000000
export MFRC522_PIN_NSS = PORT_A3
export MFRC522_PIN_SCLK = PORT_A2
export MFRC522_PIN_MISO = PORT_A0
export MFRC522_PIN_MOSI = PORT_A1
export MFRC522_PIN_RST = PORT_A7
export USART_BAUD = 9600
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
