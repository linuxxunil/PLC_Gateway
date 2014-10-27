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
	int status = i2cReadByte(I2CBUS,index,0x01,&value);
	return (status==SUCCESS)?value:status;
}

int setAO(AO index,unsigned char value)
{
	unsigned char CONTROL_DATA = 0x40;
	unsigned char block[4];
	block[0] = value;
	int status = i2cWriteBytes(I2CBUS,index,CONTROL_DATA,block,1);
	return (status==SUCCESS)?SUCCESS:status;
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