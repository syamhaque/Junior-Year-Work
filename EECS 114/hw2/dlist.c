#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "timer.c"

struct Node{
	int key;
	struct Node *prev;
	struct Node *next;
};
typedef struct Node Node;

struct Node *front;

Node *createNewNode(int x){

	Node *q = (Node*)malloc(sizeof(Node));
	
	q->key = x;
	q->prev = NULL;
	q->next = NULL;
	return q;

}

void addNodeFront(int x){
	
	Node *q = createNewNode(x);

	if(front == NULL){
		front = q;
		return;
	}
	front->prev = q;
	q->next = front;
	front = q;

}

void addNodeBack(int x){

	Node *tmp = front;
	Node *q = createNewNode(x);
	if(front == NULL){
		front = q;
		return;
	}
	while(tmp->next != NULL) tmp = tmp->next;
	tmp->next = q;
	q->prev = tmp;

}

void removeNode(Node *q){

	if(front == NULL){				//theta(1)
		return;
	}
	if(front == q){					//theta(1)
		front = q->next;			//theta(1)
	}	
	if(q->next != NULL){			//theta(1)
		q->next->prev = q->prev;	//theta(1)
	}
	if(q->prev != NULL){			//theta(1)
		q->prev->next = q->next;	//theta(1)
	}
	free(q);						//theta(1)
	return;

}

Node *searchList(int key){

	if(front == NULL){							//theta(1)
		return NULL;							//theta(1)
	}

	Node *tmp = front;							//theta(1)
	for(tmp; tmp != NULL; tmp = tmp->next){		//theta(n)
		if(tmp->key == key){					//theta(1)
			return tmp;							//theta(1)
		}
	}

	return NULL;								//theta(1)
}

Node *getFirstNode(){
	return front;
}

Node *getLastNode(Node *q){
	while(q->next != NULL) q = q->next;
	return q;
}

Node *getPrevNode(Node *q){
	return q->prev;
}

Node *getNextNode(Node *q){
	return q->next;
}

void Print() {
	struct Node* temp = front;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%d ",temp->key);
		temp = temp->next;
	}
	printf("\n");
}

int main(){

	front = NULL; //create an empty list
	
	int i, n, x, t = 10000;
	srand(time(NULL));

	stopwatch_init ();
  	struct stopwatch_t* timer = stopwatch_create (); assert (timer);

  	printf("Enter n: "); scanf("%d", &n);

	//add integers from 1 to n to list
	for(i = 1; i <= n; i++){
		addNodeBack(i);
	}

	stopwatch_start (timer); //get time for remove operation
	for(i = 0; i < t; i++){
		//generate random integer between 1 to n
		x = rand() % n + 1;

		//search for x and remove node containing x
		removeNode(searchList(x));
		addNodeFront(x);
	}
	long double t_s = stopwatch_stop (timer);
	printf ("Remove Node: %Lg seconds ==> %Lg average\n", t_s, t_s/t);

	stopwatch_destroy (timer);
	return 0;
}