#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "i2cbusses.h"
#include "i2c_status.h"
#include "status.h"

#define I2C_SMBUS_BLOCK_MAX 32

static int check_funcs(int file, int size, int daddress, int pec)
{
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	switch (size) {
	case I2C_SMBUS_BYTE:
		if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus receive byte");
			return -1;
		}
		if (daddress >= 0
		 && !(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
			return -1;
		}
		break;

	case I2C_SMBUS_BYTE_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus read byte");
			return -1;
		}
		break;

	case I2C_SMBUS_WORD_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus read word");
			return -1;
		}
		break;
	}

	if (pec
	 && !(funcs & (I2C_FUNC_SMBUS_PEC | I2C_FUNC_I2C))) {
		fprintf(stderr, "Warning: Adapter does "
			"not seem to support PEC\n");
	}

	return 0;
}

int initI2C()
{
	return (system("modprobe i2c-dev") == 0)?
					SUCCESS:DEBUG(ERR_INIT_I2C_FAILURE);
}


/**
 *	i2cBus 		: i2c bus number (#i2cdetect -l
 *	i2cAddr		: i2c address (#i2cdetect -y [addr]
 *  sDataAddr	: start Data Address(1)
 */
int i2cReadByte(int i2cBus,int i2cAddr,int startDataAddr,unsigned char *byte)
{
	int status,size;
	int fd;
	char filename[20];
	char str[8];
	int pec = 0;
	
		sprintf(str,"%d",i2cBus);
	if (lookup_i2c_bus(str) < 0)
		return DEBUG(ERR_BUS_NOT_FOUND);
	
	sprintf(str,"%d",i2cAddr);
	if (parse_i2c_address(str) < 0)
		return DEBUG(ERR_ADDR_NOT_FOUND);
	
	if ( startDataAddr > 0 ) {
		if (startDataAddr < 0 || startDataAddr > 255) 
			return DEBUG(ERR_DATA_ADDR_NOT_FOUND);
		size = I2C_SMBUS_BYTE_DATA;
	}else {
		size = I2C_SMBUS_BYTE;
		startDataAddr = -1;
	}

	fd = open_i2c_dev(i2cBus, filename, sizeof(filename), 0);
	if (fd < 0
	 || check_funcs(fd, size, startDataAddr, pec)
	 || set_slave_addr(fd, i2cAddr, 0)) {
		close(fd);
		return DEBUG(ERR_OPEN_I2C_DEV_FAILURE);
	}
	
	switch (size) {
	case I2C_SMBUS_BYTE:
		if (startDataAddr >= 0) {
			*byte = i2c_smbus_write_byte(fd, startDataAddr);
			if (*byte < 0) {
				close(fd);
				return DEBUG(ERR_WRITE_I2C_FAILURE);
			}
		} 
		*byte = i2c_smbus_read_byte(fd);
	break;
	default: // I2C_SMBUS_BYTE_DATA 
		*byte = i2c_smbus_read_byte_data(fd, startDataAddr);
	break;
	}
	close(fd);

	return (*byte>=0)?SUCCESS:DEBUG(ERR_READ_I2C_FAILURE);
}

/**
 *	i2cBus 		: i2c bus number (#i2cdetect -l)
 *	i2cAddr		: i2c address (#i2cdetect -y [addr])
 *  sDataAddr	: start Data Address(s:1)
 */
int i2cReadWord(int i2cBus,int i2cAddr,int startDataAddr,unsigned int *word)
{
	int status,size;
	int fd;
	char filename[20];
	char str[8];
	int pec = 0;
	
		sprintf(str,"%d",i2cBus);
	if (lookup_i2c_bus(str) < 0)
		return DEBUG(ERR_BUS_NOT_FOUND);
	
	sprintf(str,"%d",i2cAddr);
	if (parse_i2c_address(str) < 0)
		return DEBUG(ERR_ADDR_NOT_FOUND);
	
	size = I2C_SMBUS_WORD_DATA;;
	
	fd = open_i2c_dev(i2cBus, filename, sizeof(filename), 0);
	if (fd < 0
	 || check_funcs(fd, size, startDataAddr, pec)
	 || set_slave_addr(fd, i2cAddr, 0)) {
		close(fd);
		return DEBUG(ERR_OPEN_I2C_DEV_FAILURE);
	}
	
	*word = i2c_smbus_read_word_data(fd, startDataAddr);

	close(fd);
	return (*word>=0)?SUCCESS:DEBUG(ERR_READ_I2C_FAILURE);
	
}



int i2cWriteBytes(int i2cBus,int i2cAddr,int startDataAddr,unsigned char block[],int blockLen)
{
	int status,size;
	int fd;
	char filename[20];
	char str[8];
	int pec = 0;
	
	sprintf(str,"%d",i2cBus);
	if (lookup_i2c_bus(str) < 0)
		return DEBUG(ERR_BUS_NOT_FOUND);
	
	sprintf(str,"%d",i2cAddr);
	if (parse_i2c_address(str) < 0)
		return DEBUG(ERR_ADDR_NOT_FOUND);
	
	if (startDataAddr < 0 || startDataAddr > 0xff) 
		return DEBUG(ERR_DATA_ADDR_NOT_FOUND);
	
	
	switch(blockLen) {
	case 0: size = I2C_SMBUS_BYTE;break;
	case 1: size = I2C_SMBUS_BYTE_DATA; break;
	default: size = I2C_SMBUS_I2C_BLOCK_DATA; break;
	}

	fd = open_i2c_dev(i2cBus, filename, sizeof(filename), 0);
	if (fd < 0
	 || check_funcs(fd, size, startDataAddr, pec)
	 || set_slave_addr(fd, i2cAddr, 0)) {
		close(fd);
		return DEBUG(ERR_OPEN_I2C_DEV_FAILURE);
	}
	switch (size) {
	case I2C_SMBUS_BYTE:
		status = i2c_smbus_write_byte(fd, startDataAddr);
	break;
	case I2C_SMBUS_BYTE_DATA:
		status = i2c_smbus_write_byte_data(fd, startDataAddr, block[0]);
	break;
	default:
		status = i2c_smbus_write_i2c_block_data(fd, startDataAddr, blockLen, block);
	break;
	}
	
	close(fd);
	return (status<0)?DEBUG(ERR_WRITE_I2C_FAILURE):SUCCESS;
}
