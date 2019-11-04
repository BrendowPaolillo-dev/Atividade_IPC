/*------------------------------------------------------------------------
 * Programa:
 *    Quemeusou_cliente - Cliente do jogo 'Quem eu sou?'
 *
 * Funcionalidades:
 *    - alocar um socket
 *    - conectar ao servidor
 *    - enviar/receber as jogadas
 *
 *
 * Sintaxe:    Quemeusou_cliente [host] [porta]
 *
 *    - host  - nome ou ip do computador onde o servidor est� executando
 *    - porta - n�mero da porta que o servidor est� usando
 *
 * Notas:
 *    - se host n�o for fornecido � usado o endere�o de loopback (127.0.0.1)
 *    - se porta n�o for fornecida � usado um valor de porta padr�o
 *------------------------------------------------------------------------
 */

#include <sys/types.h>              /* tipos b�sicos do GNU C */
#include <sys/socket.h>             /* fun��es, tipos e constantes para sockets (UNIX) */
#include <netinet/in.h>             /* fun��es, tipos e constantes para sockets (Internet) */
#include <netdb.h>                  /* dados sobre a rede (ex.: hosts, protocolos, ...) */

#include <stdio.h>                  /* entrada/sa�da (I/O) */
#include <string.h>                 /* manipula��o de strings */
#include <stdlib.h>                 /* exit() */
#include <unistd.h>                 /* close() */

#define IP_SERVER     "127.0.0.1"   /* especifica o IP do servidor */
#define PORTA_PADRAO  5000          /* especifica a porta padrao de conexao */
#define MAX_BUFFER    100           /* tamanho maximo do buffer */

int main(int argc, char *argv[]) {

	struct hostent *ptrh; /* ponteiro para a tabela de hosts */
	struct sockaddr_in address; /* estrutura para armazenar o IP e a porta */
	int sd; /* descritor do socket */
	int port; /* porta utilizada na comunica��o */
	char *host; /* armazena o endere�o do servidor */
	char buffer[MAX_BUFFER]; /* armazena os dados recebidos */

	/* verifica os par�metros passados por linha de comando */
	if (argc > 2) port = atoi(argv[2]); /* converte para inteiro */
	else port = PORTA_PADRAO; /* usa a porta padr�o */

	/* testa a validade da porta */
	if ((port <= 1024) || (port >= 65536)) {
		fprintf(stderr,"n�mero de porta inv�lido %s\n",argv[2]);
		exit(1);
	} //if

	/* verifica se o endere�o do servidor foi fornecido */
	if (argc > 1) host = argv[1];
	else host = IP_SERVER;

	/* testa a validade do endereco */
	ptrh = gethostbyname(host);
	printf("Host name: %s\n",(char *)ptrh);
	if (ptrh == NULL) {
		fprintf(stderr,"host inv�lido: %s\n", host);
		exit(1);
	} //if

	/* preenche com zeros a estrutura sockaddr_in */
	memset((char *)&address,0,sizeof(address));

	/* preenche a estrutura endereco */
	address.sin_family = AF_INET; /* define familia como Internet  */
	address.sin_port = htons(port); /* define a porta */
	memcpy(&address.sin_addr, ptrh->h_addr, ptrh->h_length); /* especifica o destino (IP) */

	/* cria um socket TCP */
	sd = socket(PF_INET, SOCK_STREAM, 0);

	/* testa para verificar sucesso */
	if (sd < 0) {
		fprintf(stderr, "falha na cria��o do socket\n");
		exit(1);
	} //if

	/* conecta o socket ao servidor e testa a conex�o */
	if (connect(sd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		fprintf(stderr,"falha na conex�o\n");
		exit(1);
	} //if

	/* protocolo de comunica��o (enviar e receber dados) */
	printf ("Solicitando conex�o ...\n");
	strcpy(buffer, "Solicitando conex�o ...!!!\n");
	send (sd, buffer, sizeof (buffer), 0); /* envia dados */
	recv (sd, buffer, sizeof (buffer), 0); /* recebe dados */
	printf ("Mensagem (Remota): %s\n", buffer);

	/* protocolo chat */
	printf ("------------------------------------------------\n");
	printf ("--               Quem eu sou?                 --\n");
	printf ("------------------------------------------------\n\n");
	printf("Aperte Enter para ver o tema\n->Enquanto nao receber a resposta da sua pergunta,\n-->Enter para atualizar o chat");
	printf("\n Quando receber a resposta da sua pergunta, eh sua vez novamente.\n\n");
	//send (sd, "Iniciar.", sizeof (buffer), 0); 
	char oldbuf[MAX_BUFFER];
	while (1) {
		printf("Faca uma pergunta ou tentativa: ");
		gets(buffer);


		send (sd, buffer, sizeof (buffer), 0); /* envia dados */

		if (strcmp(buffer, "sair") == 0) break;
		
		recv (sd, buffer, sizeof (buffer), 0); /* recebe dados */
		if(sizeof(buffer) > 0){
			printf ("%s\n", buffer);
		}

	} //while

	/* fecha a conex�o */
	close(sd);
    
    return 0;
} // main
