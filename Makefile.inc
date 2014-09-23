HOME = /home/PLC_Gateway

COMMON_DIR := $(HOME)/common
GPIO_DIR := $(HOME)/gpio
UART_DIR := $(HOME)/uart
MODBUS_SERVER_DIR := $(HOME)/modbus_server

CFLAGS = -I $(COMMON_DIR)

GPIO_OBJS   := $(GPIO_DIR)/gpio_api.o $(GPIO_DIR)/pi_gpio.o
COMMON_OBJS	:= $(COMMON_DIR)/status.o
