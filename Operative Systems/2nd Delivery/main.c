/*
// Projeto SO - exercicio 1, version 03
// Sistemas Operativos, DEI/IST/ULisboa 2017-18
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "mplib3.h"
#include "matrix2d.h"


typedef struct  {
  int id; 
  int num_iter; 
  int trabalhadoras;
  int col; 
  int lin;
} InfThread; 



/*--------------------------------------------------------------------
| Function: simul
| Description: Nesta funcao implementamos a troca de mensagens (rece-
              ber e enviar); assim como as operacoes responsaveis por
              calcular as temperaturas.
---------------------------------------------------------------------*/

DoubleMatrix2D *simul(DoubleMatrix2D *matrix, DoubleMatrix2D *matrix_aux, int linhas, int colunas, int numIteracoes, int id, int trabalhadoras) {

  DoubleMatrix2D *m, *aux, *tmp;
  int iter, i, j;
  double value;
  double receive_buffer[colunas];


  if(linhas < 2 || colunas < 2)
    return NULL;

  m = matrix;
  aux = matrix_aux;

  
  for (iter = 0; iter < numIteracoes; iter++) {

    for (i = 1; i < linhas - 1; i++){
      
      for (j = 1; j < colunas - 1; j++) {
        value = ( dm2dGetEntry(m, i-1, j) + dm2dGetEntry(m, i+1, j) + dm2dGetEntry(m, i, j-1) + dm2dGetEntry(m, i, j+1) ) / 4.0;
        dm2dSetEntry(aux, i, j, value);
      }
    }
                
      tmp = aux;
      aux = m;
      m = tmp;

      if(trabalhadoras!=1){

        if( (id > 1) && (id < trabalhadoras)){
          
          if(id%2!=0){
			     
            enviarMensagem(id, id-1, dm2dGetLine(m,1), sizeof(double)*(colunas)); //envia mensagem a tarefa anterior com a primeira linha
            enviarMensagem(id, id+1, dm2dGetLine(m, linhas - 2), sizeof(double)*(colunas));  //envia mensagem a tarefa a seguir com a ulltima linha
			     
            receberMensagem(id-1, id, receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa anterior
      		  dm2dSetLine(m, 0, receive_buffer);
      		 
      		  receberMensagem(id+1, id , receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa a seguir
      		  dm2dSetLine(m, linhas-1 , receive_buffer); 
          }
           

          else if(id%2==0){ 
      
            receberMensagem(id-1, id, receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa anterior
            dm2dSetLine(m, 0, receive_buffer);
          
            receberMensagem(id+1, id , receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa a seguir
            dm2dSetLine(m, linhas-1 , receive_buffer);

            enviarMensagem(id, id-1, dm2dGetLine(m,1), sizeof(double)*(colunas)); //envia mensagem a tarefa anterior com a primeira linha
            enviarMensagem(id, id+1, dm2dGetLine(m, linhas - 2), sizeof(double)*(colunas));  //envia mensagem a tarefa a seguir com a ulltima linha
          }

        }

            
        else if(id == 1){
			    
          enviarMensagem(id, id+1, dm2dGetLine(m, linhas-2), sizeof(double)*(colunas));  //envia mensagem a tarefa a seguir com a ultima linha

      		receberMensagem(id+1, id , receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa a seguir
      		dm2dSetLine(m, linhas - 1, receive_buffer);
      	}
      		
      	else if(id == trabalhadoras){
          
          if (id%2!=0){

            enviarMensagem(id, id-1,dm2dGetLine(m,1), sizeof(double)*(colunas)); //envia mensagem a tarefa anterior com a primeira linha 

            receberMensagem(id-1, id, receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa anterior
            dm2dSetLine(m, 0, receive_buffer);
          }
      		
          else if((id%2==0)){

            receberMensagem(id-1, id, receive_buffer, sizeof(double)*(colunas)); //recebe a mensagem da tarefa anterior
            dm2dSetLine(m, 0, receive_buffer);

            enviarMensagem(id, id-1,dm2dGetLine(m,1), sizeof(double)*(colunas)); //envia mensagem a tarefa anterior com a primeira linha 
          }	
        }
      }
    }
  
  return m;
}

/*--------------------------------------------------------------------
| Function: execucao_trab
| Description: recebe os argumentos por parte do utilizador e executa
              o trabalho de enviar/receber mensagens entre tarefas
              assim como iniciar a simulacao e guardar a matriz calcu-
              lada
---------------------------------------------------------------------*/

void *execucao_trab(void *args){
  int i, linhas;
  DoubleMatrix2D *matrixt, *aux, *res;
  InfThread *m = (InfThread *) args;
  double receive_buffer[m->col];
  
  matrixt = dm2dNew(m->lin ,m->col);
  
  for(linhas = 0; linhas < m->lin; linhas++){
  	receberMensagem(0, m->id, receive_buffer, sizeof(double)*(m->col));
  	dm2dSetLine(matrixt, linhas, receive_buffer);
	}
	

  aux = dm2dNew(m->lin, m->col);
  dm2dCopy(aux, matrixt); //cria uma copia auxiliar da matrix


  res = simul(matrixt, aux, m->lin, m->col, m->num_iter, m->id, m->trabalhadoras); //procede a simulacao

  //quando chega aqui já terminou a simulacao de todas as iteracoes
  
  for(i=1; i < m->lin-1; i++){
  	enviarMensagem(m->id, 0, dm2dGetLine(res,i), sizeof(double)*(m->col));//envia uma a uma as linhas da sua fatia para a matriz principal
	}	
	
  free(matrixt);
  free(aux);

  return NULL;
  
}

/*--------------------------------------------------------------------
| Function: cria_trabalhadoras
| Description: responsavel por criar tarefas trabalhadoras (numero
              indicado pelo utilizador).
---------------------------------------------------------------------*/

void* cria_trabalhadoras(DoubleMatrix2D *matrix, int N, int iteracoes, int trabalhadoras){
  int i,j,linhas,linhas_matrix, k;
  linhas_matrix = 0;
  double receive_buffer[N+2];
  InfThread *buffer;
  k = N/trabalhadoras;
  
  pthread_t *tid = (pthread_t*)malloc(sizeof(pthread_t)*(trabalhadoras));  //array para guardar o id dos escravos
  buffer = (InfThread*)malloc(sizeof(InfThread)*(trabalhadoras));
	 
  for(i=0; i < trabalhadoras ; i++){   
	  buffer[i].id = i + 1;
	  buffer[i].lin = k + 2;
	  buffer[i].col = N + 2;
	  buffer[i].trabalhadoras = trabalhadoras;
	  buffer[i].num_iter = iteracoes;
	}
   
  for(i = 0; i < trabalhadoras ; i++){
    if (pthread_create(&tid[i], NULL, execucao_trab, &buffer[i]) !=0 ) { //cria a tarefa que vai simular essa fatia
      fprintf(stderr, "\nErro ao criar a tarefa.\n");
      exit(1); 
    }
  }


  linhas_matrix = 0;

  for(i=1; i < trabalhadoras + 1; i++){

    for(j=0; j < k + 2 ; j++){
      enviarMensagem(0, i, dm2dGetLine(matrix, j + linhas_matrix), sizeof(double)*(N+2));
    }
  
  linhas_matrix += k; 
  }


  linhas_matrix = 1;
  for(i=0; i < trabalhadoras ;i++){
	  for(linhas=1; linhas < k + 1; linhas++){
		  receberMensagem(i+1, 0, receive_buffer,sizeof(double)*(N+2));	 
		  dm2dSetLine(matrix, linhas_matrix++, receive_buffer);
	  }
	}
	
	/* Esperar que os Escravos Terminem */
  for (i = 0; i < trabalhadoras; i++){
    if (pthread_join(tid[i], NULL) != 0){
      fprintf(stderr, "\nErro ao esperar por um escravo.\n"); 
      exit(1);
		}
	}
      
  dm2dPrint(matrix);//imprimir a matrix
  
  free(buffer);
  free(tid);
	
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
| Function: main
---------------------------------------------------------------------*/

int main (int argc, char** argv){
	int i;
  	

  if(argc != 9) {
    fprintf(stderr, "\nNumero invalido de argumentos.\n");
    fprintf(stderr, "Uso: heatSim N tEsq tSup tDir tInf iteracoes trab csz\n\n");
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
  int csz = parse_integer_or_exit(argv[8], "csz"); //numero de mensagens que cada canal pode memorizar 

  


  fprintf(stderr, "\nArgumentos:\n"
	" N=%d tEsq=%.1f tSup=%.1f tDir=%.1f tInf=%.1f iteracoes=%d trabalhadoras=%d canal=%d\n" ,
	N, tEsq, tSup, tDir, tInf, iteracoes, trabalhadoras, csz);

  if(N < 1 || tEsq < 0 || tSup < 0 || tDir < 0 || tInf < 0 || iteracoes < 1 || N%trabalhadoras != 0) {
    fprintf(stderr, "\nErro: Argumentos invalidos.\n"
	" Lembrar que N >= 1, temperaturas >= 0 e iteracoes >= 1 \n\n");
    return 1;
  }

  if (inicializarMPlib(csz, trabalhadoras + 1) == -1) {
    printf("Erro ao inicializar MPLib.\n"); 
      return 1;
  }

  DoubleMatrix2D *matrix;


  matrix = dm2dNew(N+2, N+2);

  if (matrix == NULL) {
    fprintf(stderr, "\nErro: Nao foi possivel alocar memoria para a matriz.\n\n");
    return -1;
  }

  for(i=0; i<N+2; i++)
    dm2dSetLineTo(matrix, i, 0);
	

  dm2dSetLineTo (matrix, 0, tSup);
  dm2dSetLineTo (matrix, N+1, tInf);
  dm2dSetColumnTo (matrix, 0, tEsq);
  dm2dSetColumnTo (matrix, N+1, tDir);

  cria_trabalhadoras(matrix, N, iteracoes, trabalhadoras);

  dm2dFree(matrix);//faz free da matrix inicial
  libertarMPlib();// liberta a todos os canais de passagem de mensagem criados

  return 0;
}


