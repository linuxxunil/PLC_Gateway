#ifndef _PI_AIO_H_
#define _PI_AIO_H_

#include "i2c_api.h"


typedef enum _AI {
	AI_01=0x48,
}AI;

typedef enum _AO {
	AO_01=0x48,
}AO;

void initAI();
void initAO();

int getAI(AI index) ;
int setAI(AO index) ;

#endif