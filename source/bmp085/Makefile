# Configuration Requested
#

TARGET_NAME = bmp085
TARGET_TYPE = lib

LIB_CSRC = bmp085.c
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
