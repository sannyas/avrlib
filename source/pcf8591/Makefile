# Configuration Requested
#
# @dep twi

TARGET_NAME = pcf8591
TARGET_TYPE = lib

LIB_CSRC = pcf8591.c
LIB_ASRC =
LIB_MODULES =
LIB_DEFS =
LIB_CONF =

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
