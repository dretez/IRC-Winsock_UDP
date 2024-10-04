#include "flags.h"


int checkflag(char* arg)
{
	int value = 0;
	
	if (*arg != '-') return value;	// 0 = not a flag
	else value++;					// 1 = short flag
	arg++;
	if (*arg == '-') value++;		// 2 = long flag
	return value;
}

void setMsg(char* inputMsg, char** storeMsg, int* flags)
{
	*flags |= 0x01;
	*storeMsg = inputMsg;
}

void setIP(char* inputIP, char** storeIP, int* flags)
{
	*flags |= 0x02;
	*storeIP = inputIP;
}

void setPort(int port, int* storePort, int* flags)
{
	*flags |= 0x04;
	*storePort = port;
}
