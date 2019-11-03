 
/*
    read_string_fifo: Processo que faz a leitura das strings
    Autores: Brendow e Higor
    Data: 04/11/2019

 */
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/serverfifo"

//fluxo principal
int main (int argc, char **argv)
{
    int fd, bytes_read;
    char buf [512];

    // cria um FIFO se inexistente
    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) {
        perror ("mkfifo");
        exit (1);
    }

    // abre um arquivo de FIFO
    if ((fd = open (SERVER_FIFO, O_RDONLY)) == -1)
        perror ("open");


     while (1) {
         // lê mensagem do FIFO
         memset (buf, '\0', sizeof (buf));
         if ((bytes_read = read (fd, buf, sizeof (buf))) == -1)
             perror ("read");
         
         if (bytes_read == 0) {
             printf("-- Não há dados na fila \n");
             close(fd);
             fd = open (SERVER_FIFO, O_RDONLY);
             continue;
         }
        //imprime a mensagem
         if (bytes_read > 0) {
             printf("Recebidos %d bytes: %s\n", bytes_read, buf);
        }
     }

     if (close (fd) == -1)
        perror ("close");
         
}
