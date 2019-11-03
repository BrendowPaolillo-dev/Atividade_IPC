/*
    soma_vetores: Faz a soma de todos elementos do vetor, executando em duas threads
    Autores: Brendow e Higor
    Data: 04/11/2019

*/

#include <unistd.h>      /* fork(), execv(), getpid(), getppid() */
#include <sys/types.h>   /* pid_t type */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>       /* perror() */
#include <sys/wait.h>    /* wait() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//tamanho da memória compartilhada
#define SHMSZ 32
//flag de marcador da thread
int t_number = 0;
//flag do pai
int parent = 0;

//funcao que faz a soma dos elementos dos vetores
void soma_int(int p1, int p2, int *shm, int *shm2){
  for (int i = p1; i <= p2; i++){
    if (i > p1){
      shm[i] = shm[i-1] + shm[i];
    }
    //atribui a flag na memória
    if (i == p2){
      shm2[t_number] = 1;
    }
  }
}

//fluxo principal
int main(){

  pid_t pid[2];
  int childpipe[2], shmid[2], pos_vec[4];
  
  int *shm1, *shm2;
  key_t key;
  
  int vec[6] = {1, 2, 3, 4, 5, 6};
  int flags[2] = {0, 0};
  int result = 0;
  
  
  //gera uma área de memória compartilhada para inserir o vetor de dados
  key = 5678;
  if ((shmid[0] = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("Erro ao tentar criar o segmento de shm (shmget).");
		exit(1);
	}
  if ((shm1 = shmat(shmid[0], NULL, 0)) == (char*)-1) {
		perror
		    ("Erro ao acoplar o segmento ao espaço de dados do programa (shmat).");
		exit(1);
	}
  //copia o vetor de dados para área da memória
  memcpy(shm1, vec, sizeof(vec));
  
  //gera uma área de memória comparilhada para inserir as flags de prontos
  key = 6200;
  if ((shmid[1] = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("Erro ao tentar criar o segmento de shm (shmget).");
		exit(1);
	}
  
  if ((shm2 = shmat(shmid[1], NULL, 0)) == (char*)-1) {
		perror
		    ("Erro ao acoplar o segmento ao espaço de dados do programa (shmat).");
		exit(1);
	}
  //copia o vetor de flags
	memcpy(shm2, flags, sizeof(flags));

  //cria um pipe para a comunicacao entre o processo pai e filhos
  if (pipe(childpipe)){
    fprintf(stderr, "Falha ao criar o Pipe.\n");
		return EXIT_FAILURE;
  }
  
  //calcula a proporcao de vetor para cada thread e atribui ao vetor de posicoes
  int n = sizeof(vec) / sizeof(int);
  pos_vec[0] = 0;
  pos_vec[1] = (n/2)-1;
  pos_vec[2] = (n/2);
  pos_vec[3] = n-1;

  for (int i = 0; i < 2; i++){
    //cria um processo filho a cada execução do loop
    pid[i] = fork();
    if (pid[i] >= 0){
      //se o processo for o filho
      if(pid[i] == 0 && parent == 0){
        //se for o primeiro filho
        if (t_number == 0){
          close(childpipe[0]);
          read(childpipe[0], pos_vec, sizeof(pos_vec));
          printf("posicao: %d, %d  processo filho: %d\n", pos_vec[0], pos_vec[1], t_number);
          soma_int(pos_vec[0], pos_vec[1], shm1, shm2);
          fflush(stdout);
          t_number++;
        }
        //se for o segundo filho
        else{
          close(childpipe[0]);
          read(childpipe[0], pos_vec, sizeof(pos_vec));
          printf("posicao: %d, %d  processo filho: %d\n", pos_vec[2], pos_vec[3], t_number);
          soma_int(pos_vec[2], pos_vec[3], shm1, shm2);
          fflush(stdout);
        }
      }
      //se for o pai, comunique as posicoes dos filhos
      else {
        parent = 1;
        close(childpipe[0]);
        write (childpipe[1], pos_vec, sizeof(pos_vec));
      }
    }
  }
  //verifica se ambas as threads acabaram a soma
  if(shm2[0] == 1 && shm2[1] == 1){
    result = shm1[pos_vec[1]] + shm1[pos_vec[3]];
    printf("Soma total dos valores do vetor: %d\n", result);
  }
	if (shmdt(shm1) == -1) {
		perror("Erro ao desacoplar a região de memória compartilhada (shmdt).");
		exit(1);
	}
  if (shmdt(shm2) == -1) {
		perror("Erro ao desacoplar a região de memória compartilhada (shmdt).");
		exit(1);
	}
  for (int i = 0; i < 2; i++){
    /* Destruição do segmento */
    if (shmctl(shmid[i], IPC_RMID, 0) == -1){
       perror("Erro ao destruir o segmento compartilhado. (shmctl).") ;
       exit(1) ;
    }
  }

  return 0;
}