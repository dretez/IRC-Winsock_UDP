/*__________________________________ readargs __________________________________
  Realiza a verifica��o e processamento dos argumentos passados ao programa
  aquando da sua execu��o.
________________________________________________________________________________*/
void readargs(int argc, char* argv[], void* attributes[], int attribLen);

short checkflag(char* arg);

void setMsg(char* inputMsg, char** storeMsg, short* flags);

void setIP(char* inputIP, char** storeIP, short* flags);

void setPort(int port, int* storePort, short* flags);

