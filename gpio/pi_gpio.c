#include <stdio.h>

#include "pi_gpio.h"

void initDO()
{
	initGpio(GPIO17);
	setGpioDirection(GPIO17, GPIO_OUTPUT);

	initGpio(GPIO18);
	setGpioDirection(GPIO18, GPIO_OUTPUT);
}

void uninitDO()
{
	uninitGpio(GPIO17);
	uninitGpio(GPIO18);
}

void initDI()
{
	initGpio(GPIO27);
	setGpioDirection(GPIO27, GPIO_INPUT);
	
	initGpio(GPIO22);
	setGpioDirection(GPIO22, GPIO_INPUT);
}

void uninitDI()
{
	uninitGpio(GPIO27);
	uninitGpio(GPIO22);
}

DIO_VAL getDI(DI index) 
{
	static GPIO_VALUE val = 0;
	getGpioValue(index,&val);
	return (DIO_VAL)val;
}

void setDO(DO index,DIO_VAL val)
{
	setGpioValue(index, val);
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