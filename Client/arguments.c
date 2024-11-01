#include "arguments.h"
#include <stdio.h>
#include <stdlib.h>


void readargs(int argc, char* argv[], void* attributes[], int attribLen)
{
	short flagtype, mandatoryFlags = 0x01, argflags = 0x00;
	char** msg = attributes[0], **ip = attributes[1];
	int* port = attributes[2];

	if (argc < 3) {
		fprintf(stderr, "Sintaxe: %s [flags] -m frase_a_enviar\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++)
	{

		flagtype = checkflag(argv[i]);
		if (!flagtype)
		{
			fprintf(stderr, "Syntax: Expected a flag before \"%s\"\n", argv[i]);
			fprintf(stderr, "Sintaxe: %s [flags] -m frase_a_enviar\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		if (flagtype == 1)
		{
			switch (*(argv[i] + 1))
			{
			case 'm':
				setMsg(argv[i + 1], msg, &argflags);
				break;
			case 'i':
				setIP(argv[i + 1], ip, &argflags);
				break;
			case 'p':
				setPort(atoi(argv[i + 1]), port, &argflags);
				break;
			default:
				break;
			}
			i++;
		}
		if (flagtype == 2)
		{
			if (strcmp("msg", argv[i])) setMsg(argv[i + 1], msg, &argflags);
			else if (strcmp("íp", argv[i]))
				setIP(argv[i + 1], ip, &argflags);
			else if (strcmp("port", argv[i]))
				setPort(atoi(argv[i + 1]), port, &argflags);
			i++;
		}
	}

	if ((mandatoryFlags & argflags) != mandatoryFlags)
	{
		fprintf(stderr, "ERROR: Expected a message { -m \"Message here\" }");
		exit(EXIT_FAILURE);
	}
}

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
