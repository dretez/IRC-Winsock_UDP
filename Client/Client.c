/*=========================== Cliente basico UDP ===============================
Este cliente destina-se a enviar mensagens passadas na linha de comando, sob
a forma de um argumento, para um servidor especifico cuja locacao e' dada
pelas seguintes constantes: SERV_HOST_ADDR (endereco IP) e SERV_UDP_PORT (porto)

O protocolo usado e' o UDP.
==============================================================================*/

#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"

#pragma comment(lib, "Ws2_32.lib")

#define SERV_HOST_ADDR "127.0.0.1"
#define SERV_UDP_PORT  6000
#define BUFFERSIZE     4096

void Abort(char* msg);

/*________________________________ main _______________________________________
*/

int main(int argc, char* argv[])
{
	SOCKET sockfd;
	int msg_len, iResult;
	struct sockaddr_in serv_addr;
	WSADATA wsaData;
	int nbytes;
	char buffer[BUFFERSIZE];

	char* msg = NULL, * ip = NULL;
	int port = SERV_UDP_PORT;

	int i, flagtype;
	short mandatoryFlags = 0x01;		/* Each bit in this variable marks a flag
									* that must be set when running the Client
									* for ex5 we'll only use 1 mandatory flag */
	short argflags = 0x00;			/* This is the variable that will be used
									* to check which mandatory flags were used */



									/*========================= TESTA A SINTAXE =========================*/

									/*
									* Apesar de isto não ser necessário para a resolução dos exercícios,
									* é algo que deve ser feito para validar se a aplicação é executada
									* corretamente
									*/
	if (argc < 2) {
		fprintf(stderr, "Sintaxe: %s [flags] -m frase_a_enviar\n", argv[0]);
		getchar();
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++)
	{
		flagtype = checkflag(argv[i]);
		if (!flagtype)
		{
			fprintf(stderr, "Syntax: Expected a flag before \"%s\"\n", argv[i]);
			fprintf(stderr, "Sintaxe: %s [flags] -m frase_a_enviar\n", argv[0]);
			getchar();
			exit(EXIT_FAILURE);
		}
		if (flagtype == 1)
		{
			switch (*(argv[i] + 1))
			{
			case 'm':
				setMsg(argv[i + 1], &msg, &argflags);
				break;
			case 'i':
				setIP(argv[i + 1], &ip, &argflags);
				break;
			case 'p':
				setPort(atoi(argv[i + 1]), &port, &argflags);
				break;
			default:
				break;
			}
			i++;
		}
		if (flagtype == 2)
		{
			if (strcmp("msg", argv[i])) setMsg(argv[i + 1], &msg, &argflags);
			else if (strcmp("íp", argv[i]))
				setIP(argv[i + 1], &ip, &argflags);
			else if (strcmp("port", argv[i]))
				setPort(atoi(argv[i + 1]), &port, &argflags);
			i++;
		}
	}

	if ((mandatoryFlags & argflags) != mandatoryFlags)
	{

		fprintf(stderr, "ERROR: Expected a message { -m \"Message here\"}");
		getchar();
		exit(EXIT_FAILURE);
	}
	/*=============== INICIA OS WINSOCKS ==============*/

	/*
	* Este é o primeiro passo que tem que ser feito quando se pretende
	* criar uma aplicação recorrendo a Windows Sockets.
	* No fundo estamos a indicar que a nossa aplicação pretende usar
	* Windows Sockets e será retornado um código de erro caso não
	* seja possível.
	*/
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		getchar();
		exit(1);
	}

	/*=============== CRIA SOCKET PARA ENVIO/RECEPCAO DE DATAGRAMAS ==============*/

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET)
		Abort("Impossibilidade de criar socket");

	/*================= PREENCHE ENDERECO DO SERVIDOR ====================*/

	memset((char*)&serv_addr, 0, sizeof(serv_addr)); /*Coloca a zero todos os bytes*/
	serv_addr.sin_family = AF_INET; /*Address Family: Internet*/
	if (argflags & 0x02 == 0x02 && ip > 0)
		serv_addr.sin_addr.s_addr = inet_addr(ip); /*IP no formato "dotted decimal" => 32 bits*/
	else
		serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); /*IP no formato "dotted decimal" => 32 bits*/
	if (argflags & 0x04 == 0x04)
		serv_addr.sin_port = htons(port); /*Host TO Netowork Short*/
	else
		serv_addr.sin_port = htons(SERV_UDP_PORT); /*Host TO Netowork Short*/

	/*====================== ENVIA MENSAGEM AO SERVIDOR ==================*/

	msg_len = strlen(msg);

	if (sendto(sockfd, msg, msg_len, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		Abort("O subsistema de comunicacao nao conseguiu aceitar o datagrama");

	printf("<CLI1>Mensagem enviada ... a entrega nao e' confirmada.\n");

	printf("Porto do socket: %d\n", ntohs(serv_addr.sin_port));

	/*========================= RECEBER MENSAGEM ===========================*/

	nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

	if (nbytes == SOCKET_ERROR)
		Abort("Erro na recepcao de datagrams");

	buffer[nbytes] = '\0'; /*Termina a cadeia de caracteres recebidos com '\0'*/

	printf("\n<CLI>Mensagem recebida {%s}\n", buffer);

	/*========================= FECHA O SOCKET ===========================*/

	closesocket(sockfd);

	printf("\n");
	exit(EXIT_SUCCESS);
}

/*________________________________ Abort________________________________________
  Mostra uma mensagem de erro e o código associado ao ultimo erro com Winsocks.
  Termina a aplicacao com "exit status" a 1 (constante EXIT_FAILURE)
________________________________________________________________________________*/

void Abort(char* msg)
{

	fprintf(stderr, "<CLI1>Erro fatal: <%s> (%d)\n", msg, WSAGetLastError());
	exit(EXIT_FAILURE);

}


