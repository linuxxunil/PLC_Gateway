HOME = /home/PLC_Gateway

COMMON_DIR := $(HOME)/common
GPIO_DIR := $(HOME)/gpio
UART_DIR := $(HOME)/uart
I2C_DIR := $(HOME)/i2c
MODBUS_SERVER_DIR := $(HOME)/modbus_server

CFLAGS = -I $(COMMON_DIR)

COMMON_OBJS	:= $(COMMON_DIR)/status.o
DIO_OBJS	:= $(GPIO_DIR)/gpio_api.o $(GPIO_DIR)/pi_dio.o
AIO_OBJS	:= $(I2C_DIR)/i2c_api.o $(I2C_DIR)/i2cbusses.o $(I2C_DIR)/pi_aio.o
