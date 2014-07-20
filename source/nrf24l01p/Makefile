# Configuration
#
# "The io pin of CE, A0-A7,B0-B7...D0-D7"
# @conf NRF24L01_PIN_CE		[ABCD][0-7]
#
# "The io pin of CSN, A0-A7,B0-B7...D0-D7"
# @conf NRF24L01_PIN_CSN	[ABCD][0-7]
#
# "The io pin of SCK, A0-A7,B0-B7...D0-D7"
# @conf NRF24L01_PIN_SCK	[ABCD][0-7]
#
# "The io pin of MOSI, A0-A7,B0-B7...D0-D7"
# @conf NRF24L01_PIN_MOSI	[ABCD][0-7]
#
# "The io pin of MISO, A0-A7,B0-B7...D0-D7"
# @conf NRF24L01_PIN_MISO	[ABCD][0-7]
#

TARGET_NAME = nrf24l01p
TARGET_TYPE = lib

LIB_CSRC = nrf24l01p.c nrf24l01p_platform.c
LIB_CONFS = NRF24L01_PIN_CE \
		NRF24L01_PIN_CSN \
		NRF24L01_PIN_SCK \
		NRF24L01_PIN_MOSI \
		NRF24L01_PIN_MISO

ifneq ($(BUILD_MODULE),true)
  export DEVICE = atmega16
  export F_CPU = 8000000
  NRF24L01_PIN_CE = A0
  NRF24L01_PIN_CSN = A1
  NRF24L01_PIN_SCK = A2
  NRF24L01_PIN_MOSI = A3
  NRF24L01_PIN_MISO = A4
endif

include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
