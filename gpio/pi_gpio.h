#ifndef _PI_GPIO_H_
#define _PI_GPIO_H_

#include "gpio_api.h"


typedef enum _DO {
	DO_01=GPIO17 ,
	DO_02=GPIO18
}DO;

typedef enum _DIO_VAL {
	DIO_OFF,
	DIO_ON
} DIO_VAL;

typedef enum _DI {
	DI_01=GPIO27,
	DI_02=GPIO22
}DI;


typedef enum _AO {
	AO_01=GPIO23,
	AO_02=GPIO24
}AO;

typedef enum _AI {
	AI_01=GPIO25,
	AI_02=GPIO04
}AI;

void initDO();
void uninitDO();
void initDI();
void uninitDI();

// Get DI Status
DIO_VAL getDI(DI index);

// Set/Get DO Status
void setDO(DO index,DIO_VAL val);
DIO_VAL getDO(DO index);


#endif