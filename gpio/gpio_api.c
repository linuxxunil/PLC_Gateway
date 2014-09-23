#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "status.h"
#include "gpio_status.h"
#include "gpio_api.h"


int initGpio(GPIO_PIN gpioPin)
{	
	int fd;
	char str[32];
	
	sprintf(str, "/sys/class/gpio/gpio%d", gpioPin);
	if ( !access(str, F_OK) )
		return SUCCESS;
	
	fd = open("/sys/class/gpio/export", O_WRONLY);
	
	if ( fd < 0 ) {
		return DEBUG(ERR_OP_EXPORT_FAILURE);
	} 
	
	sprintf(str,"%d",gpioPin);
	if ( write(fd, str, strlen(str)) < 0 ) {
		return DEBUG(ERR_WR_EXPORT_FAILURE);
	}
	close(fd);
	
	return SUCCESS;
}

int uninitGpio(GPIO_PIN gpioPin)
{	
	int fd;
	
	char str[32];
	
	sprintf(str, "/sys/class/gpio/gpio%d", gpioPin);
	if ( access(str, F_OK) )
		return SUCCESS;
	
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	
	if ( fd < 0 ) {
		return DEBUG(ERR_OP_UNEXPORT_FAILURE);
	} 
	
	sprintf(str,"%d",gpioPin);
	if ( write(fd, str, strlen(str)) < 0 ) {
		return DEBUG(ERR_WR_UNEXPORT_FAILURE);
	}
	close(fd);
	
	return SUCCESS;
}

int setGpioValue(GPIO_PIN gpioPin, GPIO_VALUE value)
{
	char string[32];
	int status,fd;

	sprintf(string, "/sys/class/gpio/gpio%d/value", gpioPin);
	
	fd = open(string, O_WRONLY);
	if ( fd < 0 ) {
		return DEBUG(ERR_OP_VALUE_FAILURE);
	} 
	
	switch ( value ){
	case GPIO_0:
		status = write(fd, "0", 1);
	break;
	case GPIO_1:
		status = write(fd, "1", 1);
	break;
	}
	
	close(fd);
	return (status < 0) ? DEBUG(ERR_WR_VALUE_FAILURE) : SUCCESS;
}

int getGpioValue(GPIO_PIN gpioPin, GPIO_VALUE *value)
{
	char string[64],tmp;
	int status,fd;
	
	sprintf(string, "/sys/class/gpio/gpio%d/value", gpioPin);
	fd = open(string, O_RDONLY);
	if ( fd < 0 ) {
		return DEBUG(ERR_OP_VALUE_FAILURE);
	} 
	
	status = read(fd, &tmp, 1);

	if ( status > 0 ) {
		if ( tmp == 0x30 ) {
			*value = GPIO_0;
		} else {
			*value = GPIO_1;
		}
	}
	close(fd);
	return (status < 0) ? DEBUG(ERR_RD_VALUE_FAILURE) : SUCCESS;
}

int setGpioDirection(GPIO_PIN gpioPin, GPIO_DIRECTION direction)
{
	char string[32];
	int status,fd;

	sprintf(string, "/sys/class/gpio/gpio%d/direction", gpioPin);
	
	fd = open(string, O_WRONLY);
	if ( fd < 0 ) {
		return DEBUG(ERR_OP_DIRECTION_FAILURE);
	} 
	
	switch ( direction ){
	case GPIO_INPUT:
		status = write(fd, "in", 2);
	break;
	case GPIO_OUTPUT:
		status = write(fd, "out", 3);
	break;
	}
	
	close(fd);
	return (status < 0) ? DEBUG(ERR_WR_DIRECTION_FAILURE) : SUCCESS;
}

int getGpioDirection(GPIO_PIN gpioPin, GPIO_DIRECTION *direction)
{
	char string[32], tmp[4];
	int status,fd;

	sprintf(string, "/sys/class/gpio/gpio%d/direction", gpioPin);
	
	fd = open(string, O_RDONLY);
	if ( fd < 0 ) {
		return DEBUG(ERR_OP_DIRECTION_FAILURE);
	} 
	
	status = read(fd, tmp, 4);
	close(fd);
	
	if ( status > 0 ) {
		if ( !strcmp (tmp, "in") ) {
			*direction = GPIO_INPUT;
		} else {
			*direction = GPIO_OUTPUT;
		}
	}

	return (status < 0) ? DEBUG(ERR_RD_DIRECTION_FAILURE) : SUCCESS;
}
