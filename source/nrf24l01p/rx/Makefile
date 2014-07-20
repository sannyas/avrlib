TARGET_NAME = usart_demo
TARGET_TYPE = prj

# for project
PRJ_CSRC = main.c 
PRJ_ASRC = 
PRJ_MODULES = avr/usart nrf24l01p
PRJ_DEFS = 

export DEVICE = atmega16
export F_CPU = 8000000
export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

export NRF24L01_PIN_CE = A0
export NRF24L01_PIN_CSN = A1
export NRF24L01_PIN_SCK = A2
export NRF24L01_PIN_MOSI = A3
export NRF24L01_PIN_MISO = A4

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
