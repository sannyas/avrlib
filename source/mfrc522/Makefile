# Configuration
# @conf MFRC522_PIN_NSS
# @note The pin NSS
# @value PORT_[ABCD][0-7]
#
# @conf MFRC522_PIN_SCLK
# @note The pin SCLK
# @value PORT_[ABCD][0-7]
#
# @conf MFRC522_PIN_MISO
# @note The pin MISO
# @value PORT_[ABCD][0-7]
# 
# @conf MFRC522_PIN_MOSI
# @note The pin MOSI
# @value PORT_[ABCD][0-7]
#
# @conf MFRC522_PIN_RST
# @note The pin RST
# @value PORT_[ABCD][0-7]
#

TARGET_NAME = mfrc522
TARGET_TYPE = lib

LIB_CSRC = mfrc522.c mfrc522_pin.c
LIB_ASRC =
LIB_MODULES =
LIB_DEFS = xxx
LIB_CONFS = MFRC522_PIN_NSS \
	   MFRC522_PIN_SCLK \
	   MFRC522_PIN_MISO \
	   MFRC522_PIN_MOSI \
	   MFRC522_PIN_RST
#LIB_DEFS = MFRC522_DEBUG


ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
  export MFRC522_PIN_NSS = PORT_A3
  export MFRC522_PIN_SCLK = PORT_A2
  export MFRC522_PIN_MISO = PORT_A0
  export MFRC522_PIN_MOSI = PORT_A1
  export MFRC522_PIN_RST = PORT_A4
endif

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
