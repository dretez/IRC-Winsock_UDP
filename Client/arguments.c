#include "arguments.h"


short checkflag(char* arg)
{
	short value = 0;
	
	if (arg[0] != '-') return value;	// 0 = not a flag
	else value++;						// 1 = short flag
	if (arg[1] == '-') value++;			// 2 = long flag
	return value;
}

void setMsg(char* inputMsg, char** storeMsg, short* flags)
{
	*flags |= 0x01;
	*storeMsg = inputMsg;
}

void setIP(char* inputIP, char** storeIP, short* flags)
{
	*flags |= 0x02;
	*storeIP = inputIP;
}

void setPort(int port, int* storePort, short* flags)
{
	*flags |= 0x04;
	*storePort = port;
}
