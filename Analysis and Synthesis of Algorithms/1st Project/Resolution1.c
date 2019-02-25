#include <stdio.h>
#include <stdlib.h>

#define TRUE 0
#define FALSE -1
#define VISITED 1
#define NOTVISITED 0
#define INSCC 2
#define UNDEFINED -1
#define SCCVISITSCCTRUE 666


unsigned int edge_list_mallocs=0, edge_list_frees=0;



struct stack {

	unsigned int* array;
	int sp;
	unsigned int size;

};


typedef struct stack stack;

typedef unsigned int vertex;


struct edge{

    vertex source;
    vertex destination;

};

typedef struct edge edge;

struct edge_list{

    edge* edgeList;
    unsigned int size;
    unsigned int count;

};

typedef struct edge_list edge_list;



struct edge_linked_list_node{

    edge e;
    struct edge_linked_list_node* next;

};


typedef struct edge_linked_list_node edge_linked_list_node;


struct graph{

    unsigned int vertexCount;
    unsigned int edgeCount;
    edge_list* pEdgeList;
    edge_linked_list_node** edgeLinkedListArray;

};

typedef struct graph graph;

struct scc_data{

    unsigned int* vertex_scc;
    unsigned int* scc_lowest_vertex;
    unsigned int scc_count;

};

typedef struct scc_data scc_data;



struct tarjan_data{

    unsigned int scc_count;
    unsigned int* vertex_visited;
    int* vertex_low;
    int* vertex_discovery;
    int next_discovery;
    stack vertex_stack;
    scc_data sccdata;

};

typedef struct tarjan_data tarjan_data;



edge newEdge(vertex source, vertex destination){

    edge E;
    E.source = source;
    E.destination = destination;
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


unsigned int tarjan_aux( graph* pG, tarjan_data* ptjdata, vertex v){

    if(  ptjdata->vertex_visited[v] == VISITED || ptjdata->vertex_visited[v] == INSCC  ){

        return ptjdata->vertex_low[v];
    }



    stackPush( &(ptjdata->vertex_stack) , v);
    ptjdata->vertex_visited[v] = VISITED;
    ptjdata->vertex_discovery[v] = ptjdata->next_discovery;
    ptjdata->next_discovery++;
    ptjdata->vertex_low[v] = ptjdata->vertex_discovery[v];


    unsigned int  neighbor_low;
        vertex edge_neighbor;

    /*edge_list el = *(pG->pal->adjList[v]);*/


    /*
    for(i=0;i<el.count;i++){
        edge_neighbor = el.edgeList[i].destination;
        if( ptjdata->vertex_visited[edge_neighbor] == INSCC ){
            continue;
        }
        neighbor_low = tarjan_aux( pG, ptjdata, edge_neighbor);
        if(neighbor_low< ptjdata->vertex_low[v]){
            ptjdata->vertex_low[v] = neighbor_low;
        }
    } */

    edge_linked_list_node* current = pG->edgeLinkedListArray[v];

    while(current != 0){

        /*printf("edge %u %u\n" ,current->e.source+1, current->e.destination+1 );*/
        edge_neighbor = current->e.destination;

        if( ptjdata->vertex_visited[edge_neighbor] == INSCC ){
            current = current->next;
            continue;
        }

        neighbor_low = tarjan_aux( pG, ptjdata, edge_neighbor);

        if(neighbor_low< ptjdata->vertex_low[v]){

            ptjdata->vertex_low[v] = neighbor_low;

        }

        current = current->next;

    }




    vertex popped = 0;

    if(ptjdata->vertex_low[v] == ptjdata->vertex_discovery[v]){

        stackPop(&(ptjdata->vertex_stack), &popped);
        ptjdata->sccdata.vertex_scc[popped] = ptjdata->scc_count;
        ptjdata->vertex_visited[popped] = INSCC;

        ptjdata->sccdata.scc_lowest_vertex[ptjdata->scc_count] = popped;




        while(popped != v){

            stackPop(&(ptjdata->vertex_stack), &popped);
            ptjdata->sccdata.vertex_scc[popped] = ptjdata->scc_count;
            ptjdata->vertex_visited[popped] = INSCC;

            if( popped < ptjdata->sccdata.scc_lowest_vertex[ptjdata->scc_count] ){
                ptjdata->sccdata.scc_lowest_vertex[ptjdata->scc_count] = popped;
            }




        }

        ptjdata->scc_count++;
        ptjdata->sccdata.scc_count++;
    }


   return ptjdata->vertex_low[v];

}




scc_data tarjan( graph* pG){


    tarjan_data* ptjdata = malloc( sizeof(tarjan_data) );
    ptjdata->scc_count = 0;
    ptjdata->vertex_visited = malloc( sizeof(unsigned int) * pG->vertexCount );
    ptjdata->vertex_discovery = malloc( sizeof(int) * pG->vertexCount );
    ptjdata->vertex_low = malloc( sizeof(int) * pG->vertexCount );
    ptjdata->sccdata.vertex_scc = malloc(sizeof(unsigned int)* pG->vertexCount);
    ptjdata->sccdata.scc_lowest_vertex = malloc(sizeof(unsigned int)* pG->vertexCount);
    ptjdata->sccdata.scc_count = 1;
    initStack( &(ptjdata->vertex_stack) , pG->vertexCount);

    unsigned int i;

    for(i=0;i<pG->vertexCount;i++){

        ptjdata->vertex_visited[i] = NOTVISITED;
        ptjdata->vertex_discovery[i] = UNDEFINED;
        ptjdata->vertex_low[i] = UNDEFINED;

    }

    for(i=0;i< pG->vertexCount;i++){


        tarjan_aux( pG,  ptjdata,  i);

    }

    ptjdata->sccdata.scc_count--;


    scc_data sccdata = ptjdata->sccdata;


    free(ptjdata->vertex_visited);
    free(ptjdata->vertex_discovery);
    free(ptjdata->vertex_low);
    free(ptjdata);
    freeStack(ptjdata->vertex_stack);


    return sccdata;

}



int graphAddEdge(graph* pG, edge e){

    edgeListAddEdge(e, pG->pEdgeList);
    linkedListAddEdge(&pG->edgeLinkedListArray[e.source], e );
    return TRUE;

}


int edgeGreaterThan( edge e1, edge e2){

    if(e1.source > e2.source)
        return 1;

    else{
        if(e1.source == e2.source){
            if(e1.destination > e2.destination)
                return 1;
        }
    }

    return 0;

}


void heapSort( edge_list EdgeList){


    unsigned int edgeArrayCount = EdgeList.count;
    edge* edgeArray = EdgeList.edgeList;
    int i = ((int)EdgeList.count) / 2;
    int child,father;
    edge t;

    while(1){

        if( i>0){
            i--;
            t = edgeArray[i];
        }
        else{

            edgeArrayCount--;
            if(edgeArrayCount == 0)
                return;
            t = edgeArray[edgeArrayCount];
            edgeArray[edgeArrayCount] = edgeArray[0];

        }

        father = i;
        child = i*2+1;
        while(child < edgeArrayCount){



            if( (child+1 < edgeArrayCount)  && edgeGreaterThan(edgeArray[child +1 ], edgeArray[child] )  ){
                child++;
            }

            if( edgeGreaterThan(edgeArray[child], t) ){
                edgeArray[father] = edgeArray[child];
                father = child;
                child = father * 2 + 1;
            }

            else{
                break;
            }

        }

        edgeArray[father]  = t;

    }

}


void graphPrintLinkedListsEdges(graph g){


    int i;

    for(i=0;i<g.vertexCount;i++){
        printLinkedListEdges( g.edgeLinkedListArray[i] );
    }


}



int proj1(){


    unsigned int vertices,edges,i,source, destination;
    edge e;

    scanf("%u",&vertices);
    scanf("%u",&edges);


    graph* pG = malloc( sizeof(graph) );
    pG[0] = initGraph(vertices,edges);




    for( i=0; i<edges; i++){

        scanf("%u", &source);
        source--;
        scanf("%u", &destination);
        destination--;
        e = newEdge( source, destination);
        graphAddEdge(pG, e);

    }

    /*graphPrintLinkedListsEdges(pG[0]);*/
    scc_data d = tarjan(pG);

    printf("%u\n",d.scc_count);

    unsigned int** scc_visits_scc = malloc(sizeof(unsigned int*)* d.scc_count );


    for(i=0;i<d.scc_count;i++){

        scc_visits_scc[i] = malloc( sizeof(unsigned int)* d.scc_count );


    }


    edge_list* pel = initEdgeList(d.scc_count*d.scc_count);
    unsigned int source_scc, destination_scc, edges_between_scc = 0 ;


    for(i=0;i<pG->pEdgeList->count;i++){

        e = getEdge(pG->pEdgeList, i);
        source_scc = d.vertex_scc[e.source];
        destination_scc = d.vertex_scc[e.destination];


        if( destination_scc != source_scc && scc_visits_scc[source_scc][destination_scc] != SCCVISITSCCTRUE){

            scc_visits_scc[source_scc][destination_scc] = SCCVISITSCCTRUE;
            edges_between_scc++;
            e = newEdge(d.scc_lowest_vertex[source_scc]+1,d.scc_lowest_vertex[destination_scc]+1);
            edgeListAddEdge(e,pel);

        }
    }

    printf("%u\n", edges_between_scc);

    free(d.scc_lowest_vertex);
    free(d.vertex_scc);


    if(edges_between_scc > 0)
        heapSort(*pel);


    for(i=0; i<pel->count && edges_between_scc >0  ;i++){
        e = getEdge(pel, i);
        printf("%u %u\n", e.source, e.destination);
    }



    for(i=0;i<d.scc_count;i++){
        free(scc_visits_scc[i]);
    }
    free(scc_visits_scc);

    freeGraph( pG[0]);
    free(pG);
    freeEdgeList(pel);



    return 0;
}




int main(){
    proj1();
    return 0;

}