#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "modbus.h"
#include "pi_gpio.h"

int init_cb(modbus_t *ctx)
{
	printf("init_cb\n");
	initDO();
	initDI();
}

int uninit_cb(modbus_t *ctx)
{
	uninitDO();
	uninitDI();
}


int read_coils_cb(modbus_t *ctx, const uint8_t function, const int start_addr,
						const int number, uint8_t *reg)
{
	int i;
	int index = start_addr;
	int table[] = {DI_01, DI_02};
	
	for (i=0; i<number; i++){
		switch (getDI(table[i])){
		case DIO_ON:	
			reg[index++] = 1;
		break;
		case DIO_OFF:
			reg[index++] = 0;
		break;
		}
	}
	return 0;
}

int write_signal_coils_cb(modbus_t *ctx,uint8_t function, uint16_t reg, uint16_t value)
{
	DO		out;
	switch ( reg ) {
	case 0x0001: out = DO_01; break;
	case 0x0002: out = DO_02; break;
	}
	
	setDO(out, value);
	
	return 0;
}

// extern for modbus server
modbus_cb_t cb = {
	init_cb,
	uninit_cb,
	read_coils_cb,
	write_signal_coils_cb
};
