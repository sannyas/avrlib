#Configuration
#
# @conf USART_BAUD
# @note Baudrate, a number
# @value \d+
#
# @conf USART_DATABIT
# @note DataBit, 5 or 6 or 7 or 8
# @value [5678]
#
# @conf USART_STOPBIT
# @note Stop bit, 1 or 2
# @value [12]
#
# @conf USART_PARITYBIT	
# @note EVEN or ODD or NONE
# @value EVEN|ODD|NONE	

TARGET_NAME = usart
TARGET_TYPE = lib

#for library
LIB_CSRC = usart.c
LIB_ASRC = 
LIB_MODULES = 
LIB_DEFS = 
LIB_CONFS = USART_BAUD \
	    USART_DATABIT \
	    USART_STOPBIT \
	    USART_PARITYBIT

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
  USART_BAUD = 9600
  USART_DATABIT = 8
  USART_STOPBIT = 1
  USART_PARITYBIT = NONE
endif


include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
