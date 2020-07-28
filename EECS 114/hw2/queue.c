#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "timer.c"

struct queueNode
{
	double key;
	struct queueNode *next;

};
typedef struct queueNode queueNode;

struct queue
{
	queueNode *head, *tail;
	int n; //size

};
typedef struct queue queue;

void createEmptyQueue(queue *q){

	q->n = 0;
	q->head = NULL;
	q->tail = NULL;

}

void Enqueue(queue *q, double x){

	queueNode *temp = (queueNode*)malloc(sizeof(queueNode));	//theta(1)
	temp->key = x;												//theta(1)
	temp->next = NULL;											//theta(1)

	if(q->tail == NULL){										//theta(1)
		q->head = q->tail = temp;								//theta(1)
	}

	q->tail->next = temp;										//theta(1)	
	q->tail = temp;												//theta(1)

	q->n++;														//theta(1)

}

double Dequeue(queue *q){

	if(q->head == NULL) return NULL;							//theta(1)
	queueNode *temp = q->head;									//theta(1)
	double x = q->head->key;									//theta(1)
	q->head = q->head->next;									//theta(1)
	q->n--;														//theta(1)
	free(temp);													//theta(1)
	return x;													//theta(1)

}

int main(){

	queue *q = (queue*)malloc(sizeof(queue));
	
	createEmptyQueue(q);

	srand(time(NULL));
	double x = 0;
	int i, n, t;

	stopwatch_init ();
  	struct stopwatch_t* timer = stopwatch_create (); assert (timer);

  	printf("Enter n: "); scanf("%d", &n);
  	printf("Enter t: "); scanf("%d", &t);

	for(int i = 0; i < n; i++){
		double x = (double)rand();
		Enqueue(q, x); //enqueue n random floating point numbers into the queue
	}

	stopwatch_start (timer); //get time for enqueue and dequeue pair
	x = Dequeue(q); //dequeue floating point number x
	
	for(i = 0; i < t; i++){
		Enqueue(q, x); //enqueue x, t times
	}
	long double t_s = stopwatch_stop (timer);
	printf ("Dequeue & Enqueue %Lg seconds ==> %Lg average\n", t_s, t_s / t);

}