# Configuration Requested
# @conf MAX_TASK
# @note The max number of task
#

TARGET_NAME = coop_sch
TARGET_TYPE = prj

PRJ_CSRC = main.c led.c cpu.c scheduler.c
PRJ_ASRC = 
PRJ_MODULES = avr/usart
PRJ_DEFS =
PRJ_CONFS =

export DEVICE=atmega16
export F_CPU=8000000
export USART_BAUD = 115200
export USART_DATABIT = 8
export USART_STOPBIT = 1
export USART_PARITYBIT = NONE

//export MAX_TASK=5

# main Makefile
include $(AVRLIB_INCLUDE_PATH)/Makefile_inc
