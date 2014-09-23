#ifndef _GPIO_API_H_
#define _GPIO_API_H_


typedef enum _GPIO_PIN {
	GPIO04=4,
	GPIO17=17,
	GPIO18=18,
	GPIO22=22,
	GPIO23=23,
	GPIO24=24,
	GPIO25=25,
	GPIO27=27
} GPIO_PIN;

typedef enum _GPIO_DIRECTION {
	GPIO_INPUT,
	GPIO_OUTPUT
} GPIO_DIRECTION;

typedef enum _GPIO_VALUE {
	GPIO_0,
	GPIO_1
} GPIO_VALUE;

int initGpio(GPIO_PIN gpioPin);
int uninitGpio(GPIO_PIN gpioPin);
int setGpioDirection(GPIO_PIN gpioPin, GPIO_DIRECTION direction);
int getGpioDirection(GPIO_PIN gpioPin, GPIO_DIRECTION *direction);
int setGpio(GPIO_PIN gpioPin, GPIO_VALUE value);
int getGpioValue(GPIO_PIN gpioPin, GPIO_VALUE *value);

#endif