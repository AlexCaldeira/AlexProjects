/*
// Projeto SO - exercicio 3
// Sistemas Operativos, DEI/IST/ULisboa 2017-18
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "matrix2d.h"


/*--------------------------------------------------------------------
| Type: thread_info
| Description: Estrutura com Informacao para Trabalhadoras
---------------------------------------------------------------------*/

typedef struct {
  int    id;
  int    N;
  int    iter;
  int    trab;
  int    tam_fatia;
  double maxD;
} thread_info;

/*--------------------------------------------------------------------
| Type: doubleBarrierWithMax
| Description: Barreira dupla com variavel de max-reduction
---------------------------------------------------------------------*/

typedef struct {
  int             total_nodes;
  int             pending[2];
  double          maxdelta[2];
  int             iteracoes_concluidas;
} DualBarrierWithMax;

/*--------------------------------------------------------------------
| Global variables
---------------------------------------------------------------------*/

DoubleMatrix2D     *matrix_copies[2];
DualBarrierWithMax *dual_barrier;
double              maxD;
char *fichS;
int periodoS ,pid, ctrlc_flag, alarm_flag, estado_escrita;
pthread_mutex_t mutex;
pthread_cond_t bwait[2];

/*--------------------------------------------------------------------
| Function: ficheiro_inicial
---------------------------------------------------------------------*/
void ficheiro_inicial(int N, double tSup, double tInf, double tEsq, double tDir){
  int i;
  FILE *matrix_ficheiro;

  matrix_ficheiro=fopen(fichS, "r+");
  DoubleMatrix2D *tmp = dm2dNew(N+2, N+2);
  matrix_copies[0] = dm2dNew(N+2, N+2);
  matrix_copies[1] = dm2dNew(N+2, N+2);

  //nesta caso nao existe ficheiro
  if (matrix_ficheiro == NULL){  

    if (matrix_copies[0] == NULL || matrix_copies[1] == NULL) {
      fprintf(stderr, "\nErro: Nao foi possivel alocar memoria para as matrizes.\n\n");
      exit(1);
    }
      
    for(i=0; i<N+2; i++)
      dm2dSetLineTo(matrix_copies[0], i, 0);

    dm2dSetLineTo (matrix_copies[0], 0, tSup);
    dm2dSetLineTo (matrix_copies[0], N+1, tInf);
    dm2dSetColumnTo (matrix_copies[0], 0, tEsq);
    dm2dSetColumnTo (matrix_copies[0], N+1, tDir);

    dm2dCopy (matrix_copies[1], matrix_copies[0]);
  }

  //neste caso existe o ficheiro
  else{
    tmp = readMatrix2dFromFile(matrix_ficheiro, N+2, N+2);
    dm2dCopy (matrix_copies[0], tmp);
    dm2dCopy (matrix_copies[1], tmp);
    fclose(matrix_ficheiro);

  }
  free(tmp);
}


/*--------------------------------------------------------------------
| Function: handler_function
---------------------------------------------------------------------*/
void handler_function(int signal, DualBarrierWithMax* b){
	switch(signal){
		
		case SIGINT:
			if(pthread_mutex_lock(&mutex) != 0) {
			   fprintf(stderr, "\nErro ao bloquear mutex\n");
			   exit(1); 
			}
			alarm_flag = 0;
			ctrlc_flag = 1;
			if(pthread_mutex_unlock(&mutex) != 0) {
			   fprintf(stderr, "\nErro ao bloquear mutex\n");
			   exit(1); 
			}
			break;
				
		case SIGALRM:
			if(pthread_mutex_lock(&mutex) != 0) {
			  fprintf(stderr, "\nErro ao bloquear mutex\n");
			  exit(1); 
			}
			alarm_flag = 1;
			if(pthread_mutex_unlock(&mutex) != 0) {
				fprintf(stderr, "\nErro ao bloquear mutex\n");
				exit(1); 
			}
			alarm(periodoS);
			break;
		}
	}

/*--------------------------------------------------------------------
| Function: dualBarrierInit
| Description: Inicializa uma barreira dupla
---------------------------------------------------------------------*/

DualBarrierWithMax *dualBarrierInit(int ntasks) {
  DualBarrierWithMax *b;
  b = (DualBarrierWithMax*) malloc (sizeof(DualBarrierWithMax));
  if (b == NULL) return NULL;

  b->total_nodes = ntasks;
  b->pending[0]  = ntasks;
  b->pending[1]  = ntasks;
  b->maxdelta[0] = 0;
  b->maxdelta[1] = 0;
  b->iteracoes_concluidas = 0;

  if (pthread_mutex_init(&mutex, NULL) != 0) {
    fprintf(stderr, "\nErro a inicializar mutex\n");
    exit(1);
  }
  if (pthread_cond_init(&bwait[0], NULL) != 0) {
    fprintf(stderr, "\nErro a inicializar variável de condição\n");
    exit(1);
  }
  if (pthread_cond_init(&bwait[1], NULL) != 0) {
    fprintf(stderr, "\nErro a inicializar variável de condição\n");
    exit(1);
  }
  return b;
}

/*--------------------------------------------------------------------
| Function: dualBarrierFree
| Description: Liberta os recursos de uma barreira dupla
---------------------------------------------------------------------*/

void dualBarrierFree(DualBarrierWithMax* b) {
  if (pthread_mutex_destroy(&mutex) != 0) {
    fprintf(stderr, "\nErro a destruir mutex\n");
    exit(1);
  }
  if (pthread_cond_destroy(&bwait[0]) != 0) {
    fprintf(stderr, "\nErro a destruir variável de condição\n");
    exit(1);
  }
  if (pthread_cond_destroy(&bwait[1]) != 0) {
    fprintf(stderr, "\nErro a destruir variável de condição\n");
    exit(1);
  }
  free(b);
}

/*--------------------------------------------------------------------
| Function: dualBarrierWait
| Description: Ao chamar esta funcao, a tarefa fica bloqueada ate que
|              o numero 'ntasks' de tarefas necessario tenham chamado
|              esta funcao, especificado ao ininializar a barreira em
|              dualBarrierInit(ntasks). Esta funcao tambem calcula o
|              delta maximo entre todas as threads e devolve o
|              resultado no valor de retorno
---------------------------------------------------------------------*/

double dualBarrierWait (DualBarrierWithMax* b, int current, double localmax) {
  int next = 1 - current;
  if (pthread_mutex_lock(&mutex) != 0) {
    fprintf(stderr, "\nErro a bloquear mutex\n");
    exit(1);
  }
  
  // decrementar contador de tarefas restantes
  b->pending[current]--;
  // actualizar valor maxDelta entre todas as threads
  if (b->maxdelta[current]<localmax)
    b->maxdelta[current]=localmax;
  // verificar se sou a ultima tarefa
  if (b->pending[current]==0) {
    // sim -- inicializar proxima barreira e libertar threads
    b->iteracoes_concluidas++;
    b->pending[next]  = b->total_nodes;
    b->maxdelta[next] = 0;
    
    if(alarm_flag == 1){
		if(pid == -1){
			pid = fork();
			if(pid == 0){
			  escreve_matrix(matrix_copies[current], fichS);
			  exit(EXIT_SUCCESS);
			}
			else if(pid > 0){
			  alarm_flag = 0;
			}
		}
		else{
		  pid = waitpid(pid, NULL, WNOHANG); //retorna 0 se estiver algum la
		  if(pid > 0){
			pid = fork();
		    if(pid == 0){
			  escreve_matrix(matrix_copies[current], fichS);
			  exit(EXIT_SUCCESS);
			}
			
			else if(pid >0){
			  alarm_flag = 0;
			}
		  }
		}
	  } 
		
      if(ctrlc_flag == 1){
	    pid = waitpid(pid, NULL,0);	
	    
		pid = fork();
		if(pid == 0){
		  escreve_matrix(matrix_copies[(b->iteracoes_concluidas+1)%2], fichS);
		  exit(EXIT_SUCCESS);
		}
			  
		else if (pid > 0){
		  pid = waitpid(pid, NULL, 0);
		  b->maxdelta[current] = maxD/2;
		}
	  }
    
    if (pthread_cond_broadcast(&bwait[current]) != 0) {
      fprintf(stderr, "\nErro a assinalar todos em variável de condição\n");
      exit(1);
    }
  }
  else {
    // nao -- esperar pelas outras tarefas
    while (b->pending[current]>0) {
      if (pthread_cond_wait(&bwait[current], &mutex) != 0) {
        fprintf(stderr, "\nErro a esperar em variável de condição\n");
        exit(1);
      }
    }
  }
  double maxdelta = b->maxdelta[current];
  if (pthread_mutex_unlock(&mutex) != 0) {
    fprintf(stderr, "\nErro a desbloquear mutex\n");
    exit(1);
  }
  return maxdelta;
}

/*--------------------------------------------------------------------
| Function: tarefa_trabalhadora
| Description: Funcao executada por cada tarefa trabalhadora.
|              Recebe como argumento uma estrutura do tipo thread_info
---------------------------------------------------------------------*/

void *tarefa_trabalhadora(void *args) {
  thread_info *tinfo = (thread_info *) args;
  int tam_fatia = tinfo->tam_fatia;
  int my_base = tinfo->id * tam_fatia;
  double global_delta = INFINITY;
  int iter = 0;

  do {
    int atual = iter % 2;
    int prox = 1 - iter % 2;
    double max_delta = 0;

    // Calcular Pontos Internos
    for (int i = my_base; i < my_base + tinfo->tam_fatia; i++) {
      for (int j = 0; j < tinfo->N; j++) {
        double val = (dm2dGetEntry(matrix_copies[atual], i,   j+1) +
                      dm2dGetEntry(matrix_copies[atual], i+2, j+1) +
                      dm2dGetEntry(matrix_copies[atual], i+1, j) +
                      dm2dGetEntry(matrix_copies[atual], i+1, j+2))/4;
        // calcular delta
        double delta = fabs(val - dm2dGetEntry(matrix_copies[atual], i+1, j+1));
        if (delta > max_delta) {
          max_delta = delta;
        }
        dm2dSetEntry(matrix_copies[prox], i+1, j+1, val);
      }
    }
    // barreira de sincronizacao; calcular delta global
    
    global_delta = dualBarrierWait(dual_barrier, atual, max_delta);
  } while (++iter < tinfo->iter && global_delta >= tinfo->maxD);

  return 0;
}

/*--------------------------------------------------------------------
| Function: parse_integer_or_exit
---------------------------------------------------------------------*/

int parse_integer_or_exit(char const *str, char const *name){
  int value;
 
  if(sscanf(str, "%d", &value) != 1) {
    fprintf(stderr, "\nErro no argumento \"%s\".\n\n", name);
    exit(1);
  }
  return value;
}

/*--------------------------------------------------------------------
| Function: die
| Description: Imprime mensagem de erro e termina execucao
---------------------------------------------------------------------*/
void die(char const *reason) {
  fprintf(stderr, "Erro: %s\n", reason);
  exit(-1);
} 

/*--------------------------------------------------------------------
| Function: parse_double_or_exit
---------------------------------------------------------------------*/

double parse_double_or_exit(char const *str, char const *name){
  double value;

  if(sscanf(str, "%lf", &value) != 1) {
    fprintf(stderr, "\nErro no argumento \"%s\".\n\n", name);
    exit(1);
  }
  return value;
}

/*--------------------------------------------------------------------
| Function: main
| Description: Entrada do programa
---------------------------------------------------------------------*/

int main (int argc, char** argv) {
  
  int tam_fatia;
  int res;
  sigset_t sigset;
  struct sigaction actionset;
  
  if(argc != 11) {
    fprintf(stderr, "\nNumero invalido de argumentos.\n");
    fprintf(stderr, "Uso: heatSim N tEsq tSup tDir tInf iteracoes\n\n");
    return 1;
  }

  /* argv[0] = program name */
  int N = parse_integer_or_exit(argv[1], "N");
  double tEsq = parse_double_or_exit(argv[2], "tEsq");
  double tSup = parse_double_or_exit(argv[3], "tSup");
  double tDir = parse_double_or_exit(argv[4], "tDir");
  double tInf = parse_double_or_exit(argv[5], "tInf");
  int iter = parse_integer_or_exit(argv[6], "iteracoes");
  int trab = parse_integer_or_exit(argv[7], "tra");
  double maxD = parse_double_or_exit(argv[8], "maxD");
  fichS = argv[9];
  periodoS = parse_integer_or_exit(argv[10], "periodoS");

  fprintf(stderr, "\nArgumentos:\n"
	" N=%d tEsq=%.1f tSup=%.1f tDir=%.1f tInf=%.1f iteracoes=%d trabalhadoras=%d desvio=%.1f fichS=%s periodoS=%d\n",
	N, tEsq, tSup, tDir, tInf, iter, trab, maxD, fichS, periodoS);

  if(N < 1 || tEsq < 0 || tSup < 0 || tDir < 0 || tInf < 0 || iter < 1 || N%trab != 0 || maxD < 0 || periodoS < 0) {
    fprintf(stderr, "\nErro: Argumentos invalidos.\n"
	" Lembrar que N >= 1, temperaturas >= 0 e iteracoes >= 1 maxD > 0 periodoS >= 0 \n\n");
    return 1;
  }

  
  ficheiro_inicial( N, tSup, tInf, tEsq, tDir);
  pid = -1;
  ctrlc_flag = 0;
  alarm_flag = 0;


  // Inicializar Barreira
  dual_barrier = dualBarrierInit(trab);
  if (dual_barrier == NULL)
    die("Nao foi possivel inicializar barreira");

  // Calcular tamanho de cada fatia
  tam_fatia = N / trab;



  // Reservar memoria para trabalhadoras
  thread_info *tinfo = (thread_info*) malloc(trab * sizeof(thread_info));
  pthread_t *trabalhadoras = (pthread_t*) malloc(trab * sizeof(pthread_t));

  if (tinfo == NULL || trabalhadoras == NULL) {
    die("Erro ao alocar memoria para trabalhadoras");
  }

  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  sigaddset(&sigset, SIGALRM);

  if(pthread_sigmask(SIG_BLOCK, &sigset, NULL)!= NULL){
  fprintf(stderr, "\nTarefa Main nao pode bloquear SIGINT ou SIGACTION .\n");
   exit(1); 
  }

  // Criar trabalhadoras
  for (int i=0; i < trab; i++) {
    tinfo[i].id = i;
    tinfo[i].N = N;
    tinfo[i].iter = iter;
    tinfo[i].trab = trab;
    tinfo[i].tam_fatia = tam_fatia;
    tinfo[i].maxD = maxD;
    res = pthread_create(&trabalhadoras[i], NULL, tarefa_trabalhadora, &tinfo[i]);
    if (res != 0) {
      die("Erro ao criar uma tarefa trabalhadora");
    }
  }
  
  
  if(pthread_sigmask(SIG_UNBLOCK, &sigset, NULL)!= NULL){
    fprintf(stderr, "\nTarefa Main nao pode desbloquear SIGINT ou SIGACTION .\n");
    exit(1); 
  }

  sigemptyset(&actionset.sa_mask);
  actionset.sa_handler = handler_function;
  
  if(sigaction(SIGINT, &actionset, NULL) < 0){
   	fprintf(stderr, "\nTarefa Main nao pode ciar o handler para o sinal .\n");
    exit(1); 
  }

  if(sigaction(SIGALRM, &actionset, NULL) < 0){
   	fprintf(stderr, "\nTarefa Main nao pode criar o hanlder para o sinal .\n");
    exit(1); 
  }  

  if(periodoS != 0)
    alarm(periodoS);
  

  // Esperar que as trabalhadoras terminem
  for (int i=0; i<trab; i++) {
    res = pthread_join(trabalhadoras[i], NULL);
    if (res != 0)
      die("Erro ao esperar por uma tarefa trabalhadora");
  }

  dm2dPrint (matrix_copies[dual_barrier->iteracoes_concluidas%2]);

  // Libertar memoria e o ficheiro
  unlink(fichS);
  dm2dFree(matrix_copies[0]);
  dm2dFree(matrix_copies[1]);
  free(tinfo);
  free(trabalhadoras);
  dualBarrierFree(dual_barrier);
  
  return 0;
}
 
