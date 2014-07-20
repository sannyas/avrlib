#Configuration
#

TARGET_NAME = adc
TARGET_TYPE = lib

#for library
LIB_CSRC = adc.c
LIB_ASRC = 
LIB_MODULES = 
LIB_DEFS = 
LIB_CONFS = 

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
endif


include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
