include ../Makefile.inc

# Compiler Flag
CFLAG 	= -std-c99

# Object link
OBJS	= $(COMMON_OBJS)
OBJS	+= $(I2C_DIR)/i2c-lib/i2cbusses.o
OBJS	+= i2c_api.o pi_aio.o

TARGET 	= i2c_test

INC	= -I $(COMMON_DIR)
INC	= -I $(I2C_DIR)/i2c-lib

# Compiler
all: $(OBJS)
	$(CC) main.c $(CFLAGS) $(OBJS) $(INC) -o $(TARGET)
	cp $(I2C_DIR)/i2c-lib/i2cbusses.o $(I2C_DIR)
	
%: %.o
	$(CC) -o $@ $^
	@$(NM) -n $@ | grep -v '\( [aUw] \)\|\(__crc_\)\|\( \$[adt]\)' > $@.map
	@$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) *.o  *.bak *~
