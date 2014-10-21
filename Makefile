
all:
	make -C common
	make -C gpio
	make -C uart
	make -C modbus_server

clean:
	make -C modbus_server clean
	make -C uart clean
	make -C gpio clean
	make -C common clean
