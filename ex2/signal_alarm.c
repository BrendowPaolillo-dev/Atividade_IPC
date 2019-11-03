/*
    signal_alarm: Gera um alarme com duração de 10s quando executada uma interrupção
    Autores: Brendow e Higor
    Data: 04/11/2019

 */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/* Flag que controla a terminação do loop. */
volatile sig_atomic_t keep_going = 1;

int N = 10, sign = 1;

//funcao que imprime a contagem regressiva do alarme
void counting(void) {
    char buf[40];
    snprintf(buf, sizeof(buf) ,"Falta %d segundos", N);
    puts(buf);
    sleep(1);
    N--;
}
//verifica se o alarme chegou ao final
void catch_alarm(int sig){
    //flag de parada do counting
    keep_going = 0;
    puts("ALARME!");
    puts("Terminou.");
}
//funcao que inicia a contagem do alarme e para o fluxo principal
void start_alarm(int sig) {
    printf("\nPeguei o sinal %d, acionar alarme\n", sig);
    //flag de parada do fluxo
    sign = 0;
    //alarme setado para 10s
    alarm(10);

    while (keep_going){
        counting();
    }

}

//fluxo principal
int main(void) {
    //define uma função para o sinal de interrupcao
    signal(SIGINT, start_alarm);
    //define uma funcao para um alarme
    signal(SIGALRM, catch_alarm);
    
    printf("Aperte ctrl+c para enviar o sinal\n");

    while(sign){}

    return EXIT_SUCCESS;
}
