/*
// Projeto SO - exercicio 1, version 03
// Sistemas Operativos, DEI/IST/ULisboa 2017-18
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include "matrix2d.h"

typedef struct {
  int id; 
  int num_iter; 
  int trabalhadoras;
  int col; 
  int lin;
  double desvio;
} InfThread;

typedef struct {
  int contador[2];
  int terminacao_dinamica_ready;
  pthread_mutex_t barreira_mutex;
  pthread_cond_t barreira_cond;
  pthread_mutex_t terminacao_dinamica_mutex;
} Barreira_struct;

DoubleMatrix2D *matrix_global[2];
pthread_t *tid;
InfThread *buffer;
Barreira_struct barreira;

void *simul(DoubleMatrix2D  *matrix_global0, DoubleMatrix2D *matrix_global1, int linhas, int colunas, int numIteracoes, int id, int trabalhadoras, double max_desvio_dado);
double maxima_diferenca(DoubleMatrix2D *matrixGlobal0, DoubleMatrix2D *matrixGlobal1, int colunas, int linhas, int iter);
void espera_por_todos(int iter, int trabalhadoras, double max_dado, int colunas, int linhas);
void *execucao_trab(void *args);
int parse_integer_or_exit(char const *str, char const *name);
double parse_double_or_exit(char const *str, char const *name);
void cria_barreira();
void destroi_barreira();

/*--------------------------------------------------------------------
| Function: simul
---------------------------------------------------------------------*/

void *simul(DoubleMatrix2D  *matrix_global0, DoubleMatrix2D *matrix_global1, int linhas, int colunas, int numIteracoes, int id, int trabalhadoras, double max_desvio_dado){
  
  int iter, i, j, linhas_matrix, k, linha1, linha2;
  double new_value, value, value_anterior, desvio_actual, max_desvio;
	
  if(linhas < 2 || colunas < 2)
    exit(1);
  
  k = colunas/trabalhadoras;
  linha1 = (id-1)*k + 1;
  linha2 = linha1 + k + 1;
  
  for (iter = 0; iter < numIteracoes; iter++) {
    for (i = linha1; i < linha2 - 1; i++){
      for (j = 1; j < colunas - 1; j++) {	 
		
        value = (dm2dGetEntry(matrix_global[iter%2], i-1, j) + dm2dGetEntry(matrix_global[iter%2], i+1, j) + dm2dGetEntry(matrix_global[iter%2], i, j-1) + dm2dGetEntry(matrix_global[iter%2], i, j+1));
        new_value = value / 4.0;
        dm2dSetEntry(matrix_global[(iter+1)%2], i, j, new_value); 
	  }
    }		    
    
	espera_por_todos(iter, trabalhadoras, max_desvio_dado, colunas, linhas);	
	if(pthread_mutex_lock(&(barreira.terminacao_dinamica_mutex)) != 0) {
      fprintf(stderr, "\nErro ao bloquear mutex\n");
      exit(1); 
    }
    if ((barreira.terminacao_dinamica_ready == 1) && (iter != numIteracoes)) {
      pthread_mutex_unlock(&(barreira.terminacao_dinamica_mutex));
      break;
    }
    pthread_mutex_unlock(&(barreira.terminacao_dinamica_mutex));
  }
  
  return NULL;
}


/*-----------------------------------------------------------------------------
| Function:    maxima_diferenca
-----------------------------------------------------------------------------*/
double maxima_diferenca(DoubleMatrix2D *matrixGlobal0, DoubleMatrix2D *matrixGlobal1, int colunas, int linhas, int iter) {
  int i, j;
  double maximo = 0, value;

  for (i = 1; i < linhas - 1; i++) {
    for (j = 1; j < colunas - 1; j++) {
      value = fabs( dm2dGetEntry(matrixGlobal1,i,j) - dm2dGetEntry(matrixGlobal0,i,j) );

      if (value > maximo)
        maximo = value;
    }
  }
  return maximo;
}


/*--------------------------------------------------------------------
| Function: espera_por_todos
---------------------------------------------------------------------*/
void espera_por_todos(int iter, int trabalhadoras, double max_dado, int colunas, int linhas){
  int i;
  double maximo;

  if(pthread_mutex_lock(&(barreira.barreira_mutex)) != 0) {
      fprintf(stderr, "\nErro ao bloquear mutex\n");
      exit(1); 
    }

    barreira.contador[iter%2]++;
    if(barreira.contador[iter%2] ==  trabalhadoras){
	  maximo = maxima_diferenca(matrix_global[(iter+1)%2], matrix_global[iter%2], colunas, linhas, iter);
	  
	  if(maximo < max_dado)
	    barreira.terminacao_dinamica_ready = 1;
	  
      barreira.contador[(iter+1)%2] = 0 ;
      if(pthread_cond_broadcast(&(barreira.barreira_cond)) != 0){
        fprintf(stderr, "\nErro ao desbloquear variavel de condicao\n");
        exit(1);
      }
    }
    
    else{
      while(barreira.contador[iter%2] <  trabalhadoras){
        if( pthread_cond_wait(&(barreira.barreira_cond), &(barreira.barreira_mutex))!= 0 ){
          fprintf(stderr, "\nErro ao esperar pela variável de condição\n");
          exit(1);
        }
      }
    }
        
    if(pthread_mutex_unlock(&barreira.barreira_mutex) != 0){
      fprintf(stderr, "\nErro ao desbloquear mutex\n");
      exit(1); 
    }
}

/*--------------------------------------------------------------------
| Function: execucao_trab
---------------------------------------------------------------------*/
void *execucao_trab(void *args){
  int i, linhas;
  InfThread *m = (InfThread *) args;

  simul(matrix_global[0], matrix_global[1], m->lin, m->col, m->num_iter, m->id, m->trabalhadoras, m->desvio); 

  return NULL;  
}

/*--------------------------------------------------------------------
| Function: parse_integer_or_exit
---------------------------------------------------------------------*/

int parse_integer_or_exit(char const *str, char const *name)
{
  int value;
 
  if(sscanf(str, "%d", &value) != 1) {
    fprintf(stderr, "\nErro no argumento \"%s\".\n\n", name);
    exit(1);
  }
  return value;
}

/*--------------------------------------------------------------------
| Function: parse_double_or_exit
---------------------------------------------------------------------*/

double parse_double_or_exit(char const *str, char const *name)
{
  double value;

  if(sscanf(str, "%lf", &value) != 1) {
    fprintf(stderr, "\nErro no argumento \"%s\".\n\n", name);
    exit(1);
  }
  return value;
}
/*--------------------------------------------------------------------
| Function: cria_barreira
---------------------------------------------------------------------*/
void cria_barreira(){
	
  barreira.contador[0] = 0;
  barreira.contador[1] = 0;
  barreira.terminacao_dinamica_ready = 0;
	
  if(pthread_mutex_init(&(barreira.barreira_mutex), NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar mutex\n");
    exit(1);     
  }
  
  if(pthread_mutex_init(&(barreira.terminacao_dinamica_mutex), NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar mutex\n");
    exit(1);     
  }

  if(pthread_cond_init(&(barreira.barreira_cond), NULL) != 0) {
    fprintf(stderr, "\nErro ao inicializar mutex\n");
    exit(1);     
  }
}
/*--------------------------------------------------------------------
| Function: destroi_barreira
---------------------------------------------------------------------*/
void destroi_barreira(){
  if(pthread_mutex_destroy(&(barreira.barreira_mutex)) != 0) {
    fprintf(stderr, "\nErro ao destruir mutex\n");
    exit(1);
  }
  if(pthread_mutex_destroy(&(barreira.terminacao_dinamica_mutex)) != 0) {
    fprintf(stderr, "\nErro ao destruir mutex\n");
    exit(1);
  }
  if(pthread_cond_destroy(&(barreira.barreira_cond)) != 0) {
    fprintf(stderr, "\nErro ao destruir mutex\n");
    exit(1);
  }
}


/*--------------------------------------------------------------------
| Function: main
---------------------------------------------------------------------*/

int main (int argc, char** argv) {

  if(argc != 9) {
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
  int iteracoes = parse_integer_or_exit(argv[6], "iteracoes");
  int trabalhadoras = parse_integer_or_exit(argv[7], "tra");
  double maxD = parse_double_or_exit(argv[8], "maxD");


  fprintf(stderr, "\nArgumentos:\n"
	" N=%d tEsq=%.1f tSup=%.1f tDir=%.1f tInf=%.1f iteracoes=%d trabalhadoras=%d desvio=%.1f\n",
	N, tEsq, tSup, tDir, tInf, iteracoes, trabalhadoras, maxD);

  if(N < 1 || tEsq < 0 || tSup < 0 || tDir < 0 || tInf < 0 || iteracoes < 1 || N%trabalhadoras != 0 || maxD < 0) {
    fprintf(stderr, "\nErro: Argumentos invalidos.\n"
	" Lembrar que N >= 1, temperaturas >= 0 e iteracoes >= 1 maxD > 0 \n\n");
    return 1;
  }
  
  int i, k;
  k = N/trabalhadoras;

  cria_barreira();
  
  matrix_global[0] = dm2dNew(N+2, N+2);
  matrix_global[1] = dm2dNew(N+2, N+2);

  if (matrix_global[0] == NULL || matrix_global[1] == NULL) {
    fprintf(stderr, "\nErro: Nao foi possivel alocar memoria para as matrizes.\n\n");
    return -1;
  }
		
  for(i=0; i<N+2; i++)
    dm2dSetLineTo(matrix_global[0], i, 0);

  dm2dSetLineTo (matrix_global[0], 0, tSup);
  dm2dSetLineTo (matrix_global[0], N+1, tInf);
  dm2dSetColumnTo (matrix_global[0], 0, tEsq);
  dm2dSetColumnTo (matrix_global[0], N+1, tDir);

  dm2dCopy (matrix_global[1], matrix_global[0]);

  tid = (pthread_t*)malloc(sizeof(pthread_t)*(trabalhadoras));  
  buffer = (InfThread*)malloc(sizeof(InfThread)*(trabalhadoras));

  for(i=0; i < trabalhadoras ; i++){   
    buffer[i].id = i + 1;
    buffer[i].lin = k + 2;
    buffer[i].col = N + 2;
    buffer[i].trabalhadoras = trabalhadoras;
    buffer[i].num_iter = iteracoes;
    buffer[i].desvio = maxD;
  }  

  for(i = 0; i < trabalhadoras ; i++){
    if (pthread_create(&tid[i], NULL, execucao_trab, &buffer[i]) !=0 ) { 
      fprintf(stderr, "\nErro ao criar a tarefa.\n");
      exit(1); 
    }
  }

  for (i = 0; i < trabalhadoras; i++){
    if (pthread_join(tid[i], NULL) != 0){
      fprintf(stderr, "\nErro ao esperar por um escravo.\n"); 
      exit(1);
    }
  }

  if(iteracoes%2 == 0)
    dm2dPrint(matrix_global[0]);

  else if(iteracoes%2 != 0)
    dm2dPrint(matrix_global[1]);

  destroi_barreira();
  free(buffer);
  free(tid);

  return 0;
}
