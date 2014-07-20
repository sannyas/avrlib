# Configuration
#
# @conf DS1302_PIN_IO	
# @note The pin IO
# @value PORT_[ABCD][0-7]
#
# @conf DS1302_PIN_RST	
# @note The pin RST
# @value PORT_[ABCD][0-7]
#
# @conf DS1302_PIN_SCLK	
# @note The pin SCLK
# @value PORT_[ABCD][0-7]
#

TARGET_NAME = ds1302
TARGET_TYPE = lib

LIB_CSRC = ds1302.c ds1302_pin.c
LIB_ASRC =
LIB_MODULES =
LIB_DEFS = DS1302_DEBUG 

LIB_CONFS = DS1302_PIN_IO \
	   DS1302_PIN_RST \
	   DS1302_PIN_SCLK

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 80000000
  export DS1302_PIN_IO = A0
  export DS1302_PIN_RST = A1
  export DS1302_PIN_SCLK = A2
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
