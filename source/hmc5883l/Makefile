# Configuration Requested
#

TARGET_NAME = hmc5883l
TARGET_TYPE = lib

LIB_CSRC = hmc5883l.c
LIB_ASRC =
LIB_MODULES =
#LIB_DEFS = HMC5883L_DEBUG 
LIB_CONF = 

ifneq ($(BUILD_MODULE),true)
  export DEVICE=atmega16
  export F_CPU = 12000000
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
