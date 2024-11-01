/*============================== Servidor basico UDP ==========================
  Este servidor UDP destina-se a mostrar os conteudos dos datagramas recebidos.
  O porto de escuta encontra-se definido pela constante SERV_UDP_PORT.
  Assume-se que as mensagens recebida sao cadeias de caracteres (ou seja,
  "strings").
===============================================================================*/

#include <stdio.h>
#include <winsock.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERV_UDP_PORT 6000
#define BUFFERSIZE     4096

void Abort(char* msg);

/*________________________________ main ________________________________________
*/

int main(int argc, char* argv[])
{
	/*=============== INICIA OS WINSOCKS ==============*/
	
	/*
	* Este é o primeiro passo que tem que ser feito quando se pretende
	* criar uma aplicação recorrendo a Windows Sockets.
	* No fundo estamos a indicar que a nossa aplicação pretende usar
	* Windows Sockets e será retornado um código de erro caso não
	* seja possível.
	*/
#ifdef _WIN32	/* WSAStartup only needs to be called on windows platforms,
				 * if this program is compiled for any other (POSIX) OS, 
				 * theres no need to call this function */
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
	}
#endif

	/*============ CRIA O SOCKET PARA RECEPCAO/ENVIO DE DATAGRAMAS UDP ============*/

	SOCKET sockfd;
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		Abort("Impossibilidade de abrir socket");

	/*=============== ASSOCIA O SOCKET AO  ENDERECO DE ESCUTA ===============*/

	struct sockaddr_in serv_addr, cli1_addr, cli2_addr;
	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;					/*Address Family: Internet*/
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /*Host TO Network Long*/
	serv_addr.sin_port = htons(SERV_UDP_PORT);		/*Host TO Network Short*/

	/*
	* Na linha seguinte vamos então efetivamente associar o socket ao porto pretendido.
	* Para isso usamos a função bind() e é onde será feita a verificação se o porto
	* já está ou não a ser utilizado. Se o porto já estiver a ser usado então a
	* aplicação será terminada e apresentamos a mensagem de erro, neste caso: 
	* "Impossibilidade de registar-se para escuta"
	*/
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		Abort("Impossibilidade de registar-se para escuta");


	/*================ PASSA A ATENDER CLIENTES INTERACTIVAMENTE =============*/
	
	/*
	* Nas linhas seguintes vamos atender os clientes.
	* Estamos a usar um ciclo "infinito" (ciclo while) pois o objetivo do servidor
	* é estar sempre a atender clientes.
	*/

	while (1) {
		fprintf(stderr, "<SER1>Esperando datagrama...\n");
		
		char buffer[BUFFERSIZE];
		int length_addr = sizeof(cli1_addr);
		int nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cli1_addr, &length_addr);

		if (nbytes == SOCKET_ERROR)
			Abort("Erro na recepcao de datagrams");

		if (nbytes < sizeof(buffer))
			buffer[nbytes] = '\0';

		printf("\n<SER1>Mensagem recebida {%s}\n", buffer);
		printf("IP do Cliente: %s\n", inet_ntoa(cli1_addr.sin_addr));

		/*====================== ENVIA MENSAGEM AO CLIENTE ==================*/

		if (sendto(sockfd, (char *)&nbytes, sizeof(nbytes), 0, (struct sockaddr*)&cli1_addr, sizeof(cli1_addr)) == SOCKET_ERROR)
		  	Abort("O subsistema de comunicacao nao conseguiu aceitar o datagrama");

		printf("<SER1>Mensagem enviada ... a entrega nao e' confirmada.\n");
	}

}

/*________________________________ Abort________________________________________
  Mostra uma mensagem de erro e o código associado ao ultimo erro com Winsocks.
  Termina a aplicacao com "exit status" a 1 (constante EXIT_FAILURE)
________________________________________________________________________________*/

void Abort(char* msg)
{
	fprintf(stderr, "<SERV>Erro fatal: <%s> (%d)\n", msg, WSAGetLastError());
	exit(EXIT_FAILURE);
}
