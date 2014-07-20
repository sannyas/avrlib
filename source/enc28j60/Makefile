# Configurations
###
# @conf ENC28J60_PIN_CS
# @note CS pin
# @value PORT[ABCD][0-7]
#
###
# @conf XPT_PIN_SCK
# @note SCK pin
# @value PORT[ABCD][0-7]
# 
###
# @conf XPT_PIN_SI
# @note SI pin
# @value PORT[ABCD][0-7]
#
###
# @conf XPT_PIN_SO
# @note SO pin
# @value PORT[ABCD][0-7]
#
###
# @conf XPT_PIN_RESET
# @note RESET pin
# @value PORT_[ABCD][0-7]
#

TARGET_NAME = enc28j60
TARGET_TYPE = lib
LIB_CSRC = enc28j60.c enc28j60_pin.c
LIB_CONFS = \
	ENC28J60_PIN_CS \
	ENC28J60_PIN_SCK \
	ENC28J60_PIN_SI \
	ENC28J60_PIN_SO \
	ENC28J60_PIN_RESET 

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000

  export ENC28J60_PIN_CS  = PORT_B4
  export ENC28J60_PIN_SI  = PORT_B5
  export ENC28J60_PIN_SCK = PORT_B7
  export ENC28J60_PIN_SO  = PORT_B6
  export ENC28J60_PIN_RESET = PORT_B0
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
