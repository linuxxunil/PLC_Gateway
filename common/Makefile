include ../Makefile.inc

# Compiler Flag
CFLAG 	= -std-c99

# Object link
OBJS	= status.o

INC	= -I $(COMMON_DIR)

# Compiler
all: $(OBJS) 
	
%: %.o
	$(CC) -o $@ $^
	@$(NM) -n $@ | grep -v '\( [aUw] \)\|\(__crc_\)\|\( \$[adt]\)' > $@.map
	@$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f *.o *.bak *~
