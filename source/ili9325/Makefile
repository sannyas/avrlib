# Configuration 
#
# @conf ILI9325_PIN_RS
# @note The pin RS
# @value PORT[ABCD][0-7]
#
# @conf ILI9325_PIN_CS
# @note The pin CS
# @value PORT[ABCD][0-7]
#
# @conf ILI9325_PIN_WR
# @note The pin WR
# @value PORT[ABCD][0-7]
#
# @conf ILI9325_PIN_RD
# @note The pin RD
# @value PORT[ABCD][0-7]
#
# @conf ILI9325_PIN_RST
# @note The pin RST
# @value PORT[ABCD][0-7]
#
# @conf ILI9325_PIN_DB
# @note The port DB
# @value PORT_[ABCD]
#
TARGET_NAME = ili9325
TARGET_TYPE = lib
LIB_CSRC = ili9325.c ili9325_pin.c
LIB_CONFS = ILI9325_PIN_DB \
	    ILI9325_PIN_RST \
	    ILI9325_PIN_RD \
	    ILI9325_PIN_WR \
	    ILI9325_PIN_CS \
	    ILI9325_PIN_RS

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
  export ILI9325_PIN_RS = PORT_A2
  export ILI9325_PIN_CS = PORT_A0
  export ILI9325_PIN_WR = PORT_A3
  export ILI9325_PIN_RD = PORT_A4
  export ILI9325_PIN_RST = PORT_A1
  export ILI9325_PIN_DB	= PORT_B
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
