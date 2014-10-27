#ifndef _PI_DIO_H_
#define _PI_DIO_H_

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
