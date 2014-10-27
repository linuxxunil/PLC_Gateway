include ../Makefile.inc
SRC_DIR=/usr/local
LIB_MODBUS = libmodbus-3.0.6

# COMPILER OBJECTS
SERVER_OBJS = user_callback.o $(DIO_OBJS) $(COMMON_OBJS) $(AIO_OBJS)

# debug 
DBG = -O3

# FLAG
CFLAGS +=  -I $(SRC_DIR)/include/modbus -I $(GPIO_DIR) -I $(I2C_DIR)
LDFLAGS = -L $(SRC_DIR)/lib -lmodbus -g

CLIENT = modbus_client
SERVER = modbus_server
TARGET = $(CLINET) $(SERVER)

all: $(SERVER_OBJS)
	$(CC) $(CFLAGS) modbus-client.c -o $(CLIENT) $(LDFLAGS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) modbus-server.c -o $(SERVER) $(LDFLAGS)

libmodbus:
	@cd $(LIB_MODBUS) ;\
	if [ ! -f Makefile ] ; then\
		./configure ;\
	fi;\
	make ;\
	make install
	
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f $(TARGET) *.elf *.gdb *.o


