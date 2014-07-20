# Configuration Requested
#
# @dep twi

TARGET_NAME = i2c
TARGET_TYPE = lib

LIB_CSRC = i2c.c twi.c
LIB_ASRC =
LIB_MODULES =
LIB_DEFS =
LIB_CONFS = TWI_FREQUENCY

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
  export TWI_FREQUENCY = 100000
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
