# Configurations
###
# @conf XPT_PIN_CS
# @note CS pin
# @value PORT[ABCD][0-7]
#
###
# @conf XPT_PIN_DCLK
# @note SCLK pin
# @value PORT[ABCD][0-7]
# 
###
# @conf XPT_PIN_DIN
# @note DIN pin
# @value PORT[ABCD][0-7]
#
###
# @conf XPT_PIN_DOUT
# @DOUT pin
# @value PORT[ABCD][0-7]
#
###
# @conf XPT_PIN_PENIRQ
# @note which extern interrupt
# @value PORT_INT[012]
#

TARGET_NAME = xpt2046
TARGET_TYPE = lib
LIB_CSRC = xpt2046.c xpt2046_pin.c xpt2046_adjust.c
LIB_CONFS = \
	    XPT_PIN_PENIRQ \
	    XPT_PIN_DOUT \
	    XPT_PIN_DIN \
	    XPT_PIN_DCLK \
	    XPT_PIN_CS

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000

  export XPT_PIN_PENIRQ   = PORT_D2
  export XPT_PIN_DOUT     = PORT_A2
  export XPT_PIN_DIN      = PORT_A3
  export XPT_PIN_DCLK     = PORT_A4
  export XPT_PIN_CS       = PORT_A5
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
