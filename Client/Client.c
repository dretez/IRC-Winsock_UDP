/*=========================== Cliente basico UDP ===============================
Este cliente destina-se a enviar mensagens passadas na linha de comando, sob
a forma de um argumento, para um servidor especifico cuja locacao e' dada
pelas seguintes constantes: SERV_HOST_ADDR (endereco IP) e SERV_UDP_PORT (porto)

O protocolo usado e' o UDP.
==============================================================================*/

#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include "arguments.h"

#pragma comment(lib, "Ws2_32.lib")

#define SERV_HOST_ADDR "127.0.0.1"
#define SERV_UDP_PORT  6000
#define BUFFERSIZE     4096
#define TIMEOUT		   30000		// 30 seconds

void Abort(char* msg);

/*________________________________ main _______________________________________
*/

int main(int argc, char* argv[])
{
	char *msg = "Shut the IntelliSense warning", * ip = SERV_HOST_ADDR;
	int port = SERV_UDP_PORT;
	void* attr[3] = {&msg, &ip, &port};

	readargs(argc, argv, attr, 3);

	/*=============== START WINSOCK ==============*/

#ifdef _WIN32	/* WSAStartup only needs to be called on windows platforms, if
				 * this program is compiled for any other (POSIX) OS, there's no
				 * need to call this function */
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(EXIT_FAILURE);
	}
#endif

	/*=============== CRIA SOCKET PARA ENVIO/RECEPCAO DE DATAGRAMAS ==============*/

	SOCKET sockfd;
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		Abort("Impossibilidade de criar socket");

	DWORD timeout = TIMEOUT; // DWORD unsigned long
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	/*================= PREENCHE ENDERECO DO SERVIDOR ====================*/

	struct sockaddr_in serv_addr;
	memset((char*)&serv_addr, 0, sizeof(serv_addr));	/*Coloca a zero todos os bytes*/
	serv_addr.sin_family = AF_INET;						/*Address Family: Internet*/
	serv_addr.sin_addr.s_addr = inet_addr(ip);			/*IP no formato "dotted decimal" => 32 bits*/
	serv_addr.sin_port = htons(port);					/*Host TO Netowork Short*/

	/*====================== ENVIA MENSAGEM AO SERVIDOR ==================*/

	int msg_len = strlen(msg);

	if (sendto(sockfd, msg, msg_len, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		Abort("O subsistema de comunicacao nao conseguiu aceitar o datagrama");

	printf("<CLI1>Mensagem enviada ... a entrega nao e' confirmada.\n");

	/*========================= RECEBER MENSAGEM ===========================*/

	int nbytes;
	struct sockaddr_in pair, sender;
	int len_addrs = sizeof(struct sockaddr_in);
	memset((char*)&pair, 0, sizeof(serv_addr));	/*Coloca a zero todos os bytes*/
	pair.sin_family = AF_INET;						/*Address Family: Internet*/
	pair.sin_addr.s_addr = inet_addr(ip);			/*IP no formato "dotted decimal" => 32 bits*/
	pair.sin_port = htons(port);					/*Host TO Netowork Short*/

	if ((nbytes = recvfrom(sockfd, (char *)&pair, sizeof(pair), 0, (struct sockaddr*)&sender, &len_addrs)) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAETIMEDOUT) Abort("Erro na recepcao de datagrams");
		else Abort("Timeout na recepcao");
	}
	printf("\n<CLI>Server: {%s:%d}\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	printf("\n<CLI>Sender: {%s:%d}\n", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port));

	if (!memcmp(&serv_addr, &sender, sizeof(struct sockaddr_in))) {
		if (sendto(sockfd, (char*)&sender, sizeof(sender), 0, (struct sockaddr*) &pair, len_addrs) == SOCKET_ERROR) {
			Abort("O subsistema de comunicacao nao conseguiu aceitar o datagrama");
		}
	}
	else
		pair = sender;

	printf("\n<CLI>Par encontrado: {%s:%d}\n", inet_ntoa(pair.sin_addr), ntohs(pair.sin_port));

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


