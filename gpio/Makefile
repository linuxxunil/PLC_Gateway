include ../Makefile.inc

# Compiler Flag
CFLAG 	= -std-c99

# Object link
OBJS	= $(COMMON_OBJS)
OBJS 	+= gpio_api.o pi_gpio.o 

TARGET 	= gpio_test

INC		= -I $(COMMON_DIR)

# Compiler
all: $(OBJS) 
	$(CC) main.c $(CFLAGS) $(OBJS) $(INC) -o $(TARGET)
	
%: %.o
	$(CC) -o $@ $^
	@$(NM) -n $@ | grep -v '\( [aUw] \)\|\(__crc_\)\|\( \$[adt]\)' > $@.map
	@$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(TARGET) *.o *.bak *~
