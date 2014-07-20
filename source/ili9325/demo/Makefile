TARGET_NAME = usart_demo
TARGET_TYPE = prj

# for project
PRJ_CSRC = main.c
PRJ_MODULES = avr/usart ili9325
PRJ_DEFS = 

export DEVICE = atmega16
export F_CPU = 8000000

export USART_BAUD = 9600
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

export ILI9325_PIN_RS = PORT_A2
export ILI9325_PIN_CS = PORT_A0
export ILI9325_PIN_WR = PORT_A3
export ILI9325_PIN_RD = PORT_A4
export ILI9325_PIN_RST = PORT_A1
export ILI9325_PIN_DB	= PORT_B

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
