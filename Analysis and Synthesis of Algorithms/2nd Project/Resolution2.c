#include <stdio.h>
#include <stdlib.h>


#define TRUE 0
#define FALSE -1
#define EK_S 0
#define EK_T 1

unsigned int edge_list_mallocs=0, edge_list_frees=0;


struct stack {
	unsigned int* array;
	int sp;
	unsigned int size;};
typedef struct stack stack;

typedef unsigned int vertex;


struct edge{
    vertex source;
    vertex destination;
    unsigned int weight};
typedef struct edge edge;


struct edge_list{
    edge* edgeList;
    unsigned int size;
    unsigned int count;};
typedef struct edge_list edge_list;



struct edge_linked_list_node{
    edge e;
    struct edge_linked_list_node* next;};
typedef struct edge_linked_list_node edge_linked_list_node;


struct graph{
    unsigned int vertexCount;
    unsigned int edgeCount;
    edge_list* pEdgeList;
    edge_linked_list_node** edgeLinkedListArray;};
typedef struct graph graph;


edge newEdge(vertex source, vertex destination, unsigned int weight){

    edge E;
    E.source = source;
    E.destination = destination;
    E.weight = weight;
    return E;

}


int edgeListAddEdge(edge E, edge_list* pEdgeList){
    if( pEdgeList->count < pEdgeList->size ){
        pEdgeList->edgeList[ pEdgeList->count ] = E;
        pEdgeList->count++;
        return TRUE;
    }
    return FALSE;
}

edge getEdge(edge_list* pel, unsigned int index){

    if( (index >= 0) && (index<pel->count) ){
        return pel->edgeList[index];
    }
    else{
        printf("invalid index");
    }
    return newEdge(0,0);
}



edge_list* initEdgeList(unsigned int size){

    edge_list* pEdgeList;
    pEdgeList = malloc( sizeof(edge_list) );
    pEdgeList->edgeList = malloc( sizeof(edge) * size );
    edge_list_mallocs++;
    pEdgeList->size = size;
    pEdgeList->count = 0;

    return pEdgeList;
}


void linkedListAddEdge( edge_linked_list_node** ppn ,edge e ){
    edge_linked_list_node* pNewNode = malloc( sizeof(edge_linked_list_node) );
    pNewNode->e = e;
    pNewNode->next = *ppn;

    *ppn = pNewNode;

}


void freeLinkedList( edge_linked_list_node* pn){
    edge_linked_list_node* current = pn;
    edge_linked_list_node* next = 0;

    while( current != 0  ){

        next = current->next;
        free(current);
        current = next;
    }
}




void printLinkedListEdges( edge_linked_list_node* pn){
    printf("linked list edges\n");
    edge_linked_list_node* node = pn;

    while( node != 0){
        printf("%u %u\n", node->e.source, node->e.destination);
        node = node->next;
    }
    printf("linked list edges end\n");
}


int initStack(stack* s, unsigned int size){
	s->size = size;
	s->sp = -1;
	s->array = malloc( sizeof(unsigned int) * size );
	return TRUE;
}

void freeStack(stack s){
    free(s.array);
}



int stackPush(stack* s, unsigned int v){
	if( s->sp < (int) (s->size-1) ){
		s->sp++;
		s->array[s->sp] = v;

		return TRUE;
	}
	return FALSE;
}


int stackPop(stack* s, unsigned int* placeholder){
	if(s->sp >= 0){

		placeholder[0] = s->array[s->sp];
		s->sp--;
		return TRUE;
	}
	return FALSE;
}


graph initGraph( unsigned int vertex_count, unsigned int edge_count){
    graph G;
    G.vertexCount = vertex_count;
    G.edgeCount = edge_count;

    G.pEdgeList = initEdgeList(edge_count);

    G.edgeLinkedListArray = malloc( sizeof( edge_linked_list_node*) * vertex_count );

    int i;

    for( i=0; i<vertex_count; i++){
        G.edgeLinkedListArray[i] = 0;
    }

    //vertice s 
    linkedListAddEdge(newEdge(),G.pEdgeList);
    //vertice t
    linkedListAddEdge(newEdge(),G.pEdgeList);

    return G;
}


void freeEdgeList( edge_list* pel){
    free(pel->edgeList);
    edge_list_frees++;
    free(pel);
}

void freeGraph( graph g){
    freeEdgeList(g.pEdgeList);
    int i;
    for(i=0; i<g.vertexCount; i++){
        freeLinkedList( g.edgeLinkedListArray[i] );
    }
    free(g.edgeLinkedListArray);
}




int proj2(){
	unsigned int linhas,colunas,elementos,contador,i,j,value,source, destination;
    edge e;

    scanf("%u",&linhas);
    scanf("%u",&colunas);
    elementos = linhas*colunas;

    graph* pG = malloc( sizeof(graph) );
	pG[0] = initGraph(vertices,edges);

	contador = 1;
	//peso de primeiro plano liga ao vertice de primeiro plano S
    for(i =  0; i < linhas; i++){
    	for(j = 0; j < colunas; j++){
    		scanf("%u", &value);
    		e = newEdge( EK_S, contador++, value);
    	}
    }
    contador = 1;
    //peso de segundo plano liga ao vertice de segundo plano T
    for(i = 0 ; i < linhas; i++){
    	for(j = 0; j < colunas; j++){
			scanf("%u", &value);
    		e = newEdge(contador++ , EK_T, value);

    	}
    }
    contador = 1 + colunas:
    //peso de vizinhança horizontal
    for(i = 1 ; i < linhas; i++){
    	for(j = 0; j < colunas; j++){
    		scanf("%u", &value);
    		e = newEdge(contador++, contador - colunas, value);

    	}
    }

    contador = 1 + linhas;
    //peso de vizinhança vertical
    for(i = 0 ; i < linhas; i++){
    	for(j = 0; j < colunas; j++){
    		scanf("%u", &value);
    		e = newEdge(contador++, contador - linhas, value);

    	}
    }
}


int main(){
	proj2();
	return 0;
}