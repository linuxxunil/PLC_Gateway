#include <stdio.h>
#include "pi_aio.h"

int main(int argv,char *argc[])
{
	initAI();
	
	printf("0x%02x\n",getAI(AI_01));
	return 0;
}
