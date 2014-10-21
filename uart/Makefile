include ../Makefile.inc
# Compiler Flag

# Object link

OBJS = $(COMMON_DIR)/status.o uart.o

TARGET = uart_test

CFLAG = -std=c99
INC = -I $(COMMON_DIR)

# Compiler
all: $(OBJS)
	$(CC) main.c $(CFLAGS) $(OBJS) $(INC) -o $(TARGET)
	
%: %.o
	$(CC) -o $@ $^
	@$(NM) -n $@ | grep -v '\( [aUw] \)\|\(__crc_\)\|\( \$[adt]\)' > $@.map
	@$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) *.o *.bak *~
