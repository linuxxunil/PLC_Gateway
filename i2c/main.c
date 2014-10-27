#include <stdio.h>
#include "pi_aio.h"

int main(int argv,char *argc[])
{
	unsigned char byte;
	unsigned int word;
	unsigned char block[32];
	initAI();
	
	block[0] = 0x00;	
	printf("%d\n",i2cWriteBytes(1,0x48,0x40,block,1));
	
	
	
	//printf("0x%02x\n",getAI(AI_01));
	return 0;
}
