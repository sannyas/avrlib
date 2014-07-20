TARGET_NAME = xpt2046_demo
TARGET_TYPE = prj

# for project
PRJ_CSRC = main.c
PRJ_MODULES = avr/usart xpt2046 ili9325
PRJ_DEFS = 

export DEVICE = atmega16
export F_CPU = 8000000

export XPT_PIN_PENIRQ   = PORT_D2
export XPT_PIN_DOUT     = PORT_C0
export XPT_PIN_DIN      = PORT_C1
export XPT_PIN_DCLK     = PORT_C6
export XPT_PIN_CS       = PORT_C7

export USART_BAUD = 115200
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
