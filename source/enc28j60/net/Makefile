# Configurations

TARGET_NAME = ixrip
TARGET_TYPE = prj
PRJ_CSRC = arp.c ixrip.c ethernet.c main.c ip.c icmp.c udp.c
PRJ_MODULES = enc28j60 avr/usart
PRJ_DEFS = ARP_DEBUG IP_DEBUG ETHERNET_DEBUG ICMP_DEBUG UDP_DEBUG
PRJ_CONFS = 

export DEVICE = atmega16
export F_CPU = 8000000

export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

export ENC28J60_PIN_CS  = PORT_B4
export ENC28J60_PIN_SI  = PORT_B5
export ENC28J60_PIN_SCK = PORT_B7
export ENC28J60_PIN_SO  = PORT_B6
export ENC28J60_PIN_RESET = PORT_B0

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
