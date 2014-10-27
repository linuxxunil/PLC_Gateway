#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#include "modbus.h"
#include "pi_dio.h"
#include "pi_aio.h"

int init_cb(modbus_t *ctx)
{
	printf("init_cb\n");
	initDO();
	initDI();
	initAI();
}

int uninit_cb(modbus_t *ctx)
{
	uninitDO();
	uninitDI();
}

/**
 * Function   : 0x01
 * Description: Read coil ( Read DO Value)
 */
int read_coils_cb(modbus_t *ctx, const uint8_t function, const int start_addr,
						const int number, uint8_t *reg)
{
	int i;
	int index = start_addr;
	int table[] = {DO_01, DO_02};
	
	for (i=0; i<number; i++){
		switch (getDO(table[i])){
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

/**
 * Function   : 0x02
 * Description: Read coil ( Read DO Value)
 */
int read_discrete_input_cb(modbus_t *ctx, const uint8_t function, const int start_addr,
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

/** 
 *  Function  : 0x03
 *  Descrpiton: Read holding registers (Read AO)
 */
int read_holding_register_cb(modbus_t *ctx, const uint8_t function, const int start_addr,
					const int number, uint16_t *reg)
{	
	return 0;
}


/** 
 *  Function  : 0x04
 *  Descrpiton: read input register (Read AI)
 */
 int read_input_register_cb(modbus_t *ctx, const uint8_t function, const int start_addr,
						const int number, uint16_t *reg)
{
	int i;
	int status = getAI(AI_01);
	
	for (i = start_addr; i < start_addr + number; i++) {
		reg[i] = status;
	}
	//printf("0x%02x\n",status);
	
	//reg[0] = 0x2500;
	//printf("%02x\n",reg[0]);
	return 0;
}


/** 
 *  Function  : 0x05
 *  Descrpiton: write coils (write DO)
 */
int write_signal_coil_cb(modbus_t *ctx,uint8_t function, uint16_t reg, uint16_t value)
{
	DO		out;
	switch ( reg ) {
	case 0x0001: out = DO_01; break;
	case 0x0002: out = DO_02; break;
	}
	
	setDO(out, value);
	
	return 0;
}

/** 
 *  Function  : 0x06
 *  Descrpiton: write single register (write AO)
 */
int write_signal_register_cb(modbus_t *ctx,uint8_t function, uint16_t reg, uint16_t value)
{	
	AI		in;
	switch ( reg ) {
	case 0x0001: in = AI_01; break;
	}
	return setAO(in, value);
}




// extern for modbus server
modbus_cb_t cb = {
	init_cb,
	uninit_cb,
	read_coils_cb,					//0x01
	read_discrete_input_cb,			//0x02
	read_holding_register_cb,		//0x03
	read_input_register_cb,			//0x04
	write_signal_coil_cb,			//0x05
	write_signal_register_cb		//0x06
};
