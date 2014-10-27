#include <stdio.h>
#include "pi_aio.h"
#include "status.h"

#define I2CBUS	1

void initAI()
{
	initI2C();
}

void initAO()
{
	initI2C();
}

int getAI(AI index) 
{
	unsigned char value;
	int status = i2cReadByte(I2CBUS,index,1,&value);
	return (status==SUCCESS)?value:status;
}


/*
int main(int argv,char *argc[])
{
	//initGpio(GPIO04);
	setGpioDirection(GPIO04, GPIO_OUTPUT);
	setGpioValue(GPIO04, GPIO_0);
	
	
	return 0;
}
*/