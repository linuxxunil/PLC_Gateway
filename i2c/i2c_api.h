#ifndef _I2C_API_H_
#define _I2C_API_H_

int initI2C();
int readByte(int i2cBus,unsigned int i2cAddr,int which);
int i2cReadWord(int i2cBus,int i2cAddr,int startDataAddr,unsigned int *word);
int i2cWriteBytes(int i2cBus,int i2cAddr,int startDataAddr,unsigned char block[],int blockLen);
#endif