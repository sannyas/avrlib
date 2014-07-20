TARGET_NAME = tx_demo
TARGET_TYPE = prj

#for library
PRJ_CSRC = main.c 
PRJ_MODULES = avr/usart nrf24l01

# configuration
  export DEVICE = atmega16
  export F_CPU = 8000000
  export NRF24L01_PIN_CE = A0
  export NRF24L01_PIN_CSN = A1
  export NRF24L01_PIN_SCK = A2
  export NRF24L01_PIN_MOSI = A3
  export NRF24L01_PIN_MISO = A4
  export USART_BAUD = 9600
  export USART_DATABIT = 8
  export USART_STOPBIT = 1
  export USART_PARITYBIT = NONE

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
