/**
 *  \file prim.c
 *  Mohammed Haque 62655407
    2/22/19
 *  \brief Implement your Prim's algorithm for MST
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "timer.c"

//node for adjacency list
struct AdjacencyListNode
{
	int destination;
	float weight;
	AdjacencyListNode *next;
}; typedef struct AdjacencyListNode ALNode;

struct AdjacencyList
{
	ALNode *head;
	ALNode *tail;
}; typedef struct AdjacencyList AL;

//array of adjacency lists with size V, amount of vertices
struct Graph
{
	int V;
	AL *ALarray;
}; typedef struct Graph Graph;

//node for min heap
struct MinHeapNode
{
	float key;
	int v;
}; typedef struct MinHeapNode MHNode;

struct MinHeap
{
	int size;
	int max;
	int *nodePos;
	MHNode **MHarray;
}; typedef struct MinHeap MH;

//create and initialize node for adjacency list
ALNode *createALNode(int destination, float weight){

	ALNode *q = (ALNode*)malloc(sizeof(ALNode));
	q->destination = destination;
	q->weight = weight;
	q->next = NULL;
	
	return q;
}

//create and initialize array of adjacency lists
Graph *createGraph(int V){

	int i;
	Graph *g = (Graph*)malloc(sizeof(Graph));
	g->V = V;
	g->ALarray = (AL*)malloc(sizeof(AL) * V);

	for(i = 0; i < V; i++){
		g->ALarray[i].head = NULL;
		g->ALarray[i].tail = NULL;
	}	

	return g;
}

//create and initialize node for min heap
MHNode *createMHNode(float key, int v){

	MHNode *q = (MHNode*)malloc(sizeof(MHNode));
	q->key = key;
	q->v = v;

	return q;
}

//create and initialize min heap
MH *createMH(int max){

	MH *e = (MH*)malloc(sizeof(MH));
	e->size = 0;
	e->max = max;
	e->nodePos = (int*)malloc(sizeof(int) * max);
	e->MHarray = (MHNode**)malloc(sizeof(MHNode*) * max);

	return e;
}

//add eadge to undirected graph
void createEdge(Graph *g, int source, int destination, float weight){

	assert(g);
	ALNode *q = createALNode(destination, weight);	//node q created to add edge from source to destination
	q->next = g->ALarray[source].head;				//node q added to adjacency list
	g->ALarray[source].head = q;					//node q added to beginning

	q = createALNode(source, weight);				//another node q created to add edge from destination to source
	q->next = g->ALarray[destination].head;
	g->ALarray[destination].head = q;

}

//heapify with index given and update position of nodes
void minHeapify(MH *e, int index){

	assert(e);
	int left = 2*index+1, right = 2*index+2, lowest =  index;

	if((left < e->size) && (e->MHarray[left]->key < e->MHarray[lowest]->key))	lowest = left;
	if((right < e->size) && (e->MHarray[right]->key < e->MHarray[lowest]->key))	lowest = right;
	//swap nodes of min heap
	if(lowest != index){
		MHNode *l = e->MHarray[lowest];
		MHNode *i = e->MHarray[index];

		e->nodePos[l->v] = index;
		e->nodePos[i->v] = lowest;

		MHNode *temp = e->MHarray[lowest];
		e->MHarray[lowest] = e->MHarray[index];
		e->MHarray[index] = temp;

		minHeapify(e, lowest);
	}

}

//extract minimum node from the heap
MHNode *getMinNode(MH *e){

	assert(e);
	if(e->size == 0)	return NULL;

	MHNode *q = e->MHarray[0];				//place root node into new node q
	MHNode *n = e->MHarray[e->size-1];		//set root node to last node
	e->MHarray[0] = n;

	e->nodePos[q->v] = e->size-1;			//update position of last node
	e->nodePos[n->v] = 0;
	
	e->size--;								//reduce min heap size
	minHeapify(e, 0);						//heapify root node

	return q;
}

//decrease key value of vertex v
void decKey(MH *e, float key, int v){

	assert(e);
	int i = e->nodePos[v];			//get position of v 
	e->MHarray[i]->key = key;		//get node and change key value

	for(i; i && (e->MHarray[i]->key < e->MHarray[(i-1)/2]->key); i = (i-1)/2){		//go up tree by moving to parent index
		//swap nodes
		e->nodePos[e->MHarray[i]->v] = (i-1)/2;
		e->nodePos[e->MHarray[(i-1)/2]->v] = i;
		MHNode *temp = e->MHarray[i];
		e->MHarray[i] = e->MHarray[(i-1)/2];
		e->MHarray[(i-1)/2] = temp;
	}

}

//prim's algorithm that creates MST
void primMST(Graph *g, int check){

	assert(g);		
	int V = g->V;				//number of vertices
	int u, v;
	MH *e = createMH(V);		
	float key[V];				//key values for min edge weight
	int pi[V];					//array to store MST
	//initialize min heap
	for(v = 1; v < V; v++){
		pi[v] = -1;
		key[v] = FLT_MAX;
		e->MHarray[v] = createMHNode(key[v], v);
		e->nodePos[v] = v;
	}
	//first key value set to 0 to be extracted
	key[0] = 0;
	e->MHarray[0] = createMHNode(key[0], 0);
	e->nodePos[0] = 0;
	e->size = V;		//size of min heap is V

	while(e->size != 0){
		MHNode *q = getMinNode(e);				//get vertext w min key value
		u = q->v;								//store vertex
		ALNode *n = g->ALarray[u].head;			//go thru adjacent vertices of vertex 
		while(n != NULL){
			v = n->destination;
			if((e->nodePos[v] < e->size) && (n->weight < key[v])){			//if v not in and u-v weight is less than key value v then update values
				pi[v] = u;
				key[v] = n->weight;
				decKey(e, key[v], v);
			}
			n = n->next;
		}
	}
	//if dense input then output dense MST
	if(check == 0){
		FILE *dense;
		dense = fopen("denseMST.txt","w");
		for(int i = 1; i < V; i++){
			fprintf(dense, "%d %d %0.4f\n", i, pi[i], key[i]);
		}
	}else{	//if sparse input then output sparse MST
		FILE *sparse;
		sparse = fopen("sparseMST.txt","w");
		for(int i = 1; i < V; i++){
			fprintf(sparse, "%d %d %0.4f\n", i, pi[i], key[i]);
		}
	}
}

//main gets the running time of prim for dense and sparse input by reading input file to get required values and running prim's algorithm
int main() { 
    
    int V, source, destination;
    float weight;
    int i;
    FILE *dense, *sparse;

    stopwatch_init ();
  	struct stopwatch_t* timer = stopwatch_create (); assert (timer);

    dense = fopen("dense_100000.txt","r");
    fscanf(dense, "%d\n", &V);
    Graph* gDense = createGraph(V);
    while(!feof(dense)){
		fscanf(dense, "%d %d %f\n", &source, &destination, &weight);
		createEdge(gDense, source, destination, weight);
    }
    stopwatch_start (timer);
  	primMST(gDense, 0); 
  	long double t = stopwatch_stop (timer);
  	printf ("Dense test graph running time = %Lg seconds\n", t);
  	fclose(dense);

  	sparse = fopen("sparse_100000.txt","r");
    fscanf(sparse, "%d\n", &V);
    Graph* gSparse = createGraph(V);
    while(!feof(sparse)){
		fscanf(sparse, "%d %d %f\n", &source, &destination, &weight);
		createEdge(gSparse, source, destination, weight);
    }
    stopwatch_start (timer);
  	primMST(gSparse, 1);
  	t = stopwatch_stop (timer);
  	printf ("Sparse test graph running time = %Lg seconds\n", t);
  	fclose(sparse);

  	free(gDense);
  	free(gSparse);
  	gDense = NULL;
  	gSparse = NULL;
  
    return 0; 
} 