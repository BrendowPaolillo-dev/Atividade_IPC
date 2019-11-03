 
/*
    write_string_fifo: Processo que faz a escrita das strings
    Autores: Brendow e Higor
    Data: 04/11/2019

 */
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>

#define SERVER_FIFO "/tmp/serverfifo"
#define N 2

char buf [512];

//fluxo principal
int main (int argc, char **argv)
{
    int fd_server;
    //abre uma fila para escrita
    if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) {
        perror ("open: server fifo");
        return 1;
    }

    int value = 0;
    while (value < N) {
        value++;
        printf("Escreva a string: ");
        //lê uma string da entrada para escrever
        scanf("%s", buf);
        printf("Sending: %s\n", buf);

        // envia mensagem para o fifo
        write (fd_server, buf, strlen (buf));
    }
        
   if (close (fd_server) == -1) {
       perror ("close");
       return 2;
   }

          
    return 0;
}
