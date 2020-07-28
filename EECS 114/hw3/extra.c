/**
 *  \file extra.c
 *  Mohammed Haque 62655407
    2/22/19
 *  \brief Implement your Hashing with chaining
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

struct HashNode
{
	int key;
	int value;
	HashNode *next;
}; typedef struct HashNode HashNode;

struct HashTable
{
	HashNode *head;
	HashNode *tail;
};

HashTable *hashArray;
HashNode *item;
int reprobes;

//generate hash function using multiplicaiton method
int hashFn(int key, int size){
	return size * ((key*0.6180339887) - (int)(key*0.6180339887));
}

//create and initialize hash table
void createHashTable(int size){

	hashArray = (HashTable*)malloc(sizeof(HashTable) * size);
	item = 0;
	reprobes = 0;

	for(int i = 0; i < size; i++){
		hashArray[i].head = NULL;
		hashArray[i].tail = NULL;
	}

	assert(hashArray);

}

//search for key using the hash function
struct HashNode *search(int key, int size){

	int index = hashFn(key, size);

	HashNode *q = (HashNode*)hashArray[index].head;		//create temp pointer to list head at index of array
	while(q != NULL){									//while list isn't empty search for key	
		if(q->key == key){								//if index returns key then key is found
			return q;
		}
		q = q->next;									//iterate thru list
		index++;
		reprobes++;
	}

	return NULL;
}

//insert value at index found using hash function and key
void insert(int key, int value, int size){

	int index = hashFn(key, size);

	HashNode *q = (HashNode*)hashArray[index].head;		//create temp pointer to list head at index of array
	item = (HashNode*)malloc(sizeof(HashNode));			//allocate memory for item to be inserted

	//set key and value for item to ones going to be inserted
	item->key = key;	
	item->value = value;
	item->next = NULL;

	if(q == NULL){										//if list is empty insert item to head
		hashArray[index].head = item;
		hashArray[index].tail = item;
	}
	else{												//if list has elements insert item to tail (the first empty position)
		hashArray[index].tail->next = item;
		hashArray[index].tail = item;
		reprobes++;
	}

}

int main(){

	int key, value, size;
	double avgReprobes;

	printf("What size is the HashTable?\n");
	scanf("%d", &size);
	
	//fill up 50% of hash table and get avg reprobes
	createHashTable(size); avgReprobes = 0;
	for(int i = 0; i < size*0.5; i++){
		key = rand();
		value = rand() % 100000 + 1;
		insert(key, value, size);
	}
	avgReprobes = reprobes/(size*0.5);
	printf("50%% load avg insert reprobes = %f\n", avgReprobes);
	reprobes = 0; avgReprobes = 0;
	//search for 10000 numbers in a 50% full hash table and get avg reprobes
	for(int i = 0; i < 10000; i++){
		key = rand();
		value = rand() % 100000 + 1;
		item = search(key, size);
	}
	avgReprobes = reprobes/10000.00;
	printf("50%% load avg search reprobes = %f\n", avgReprobes);

	free(hashArray);
	free(item);

	//fill up 90% of hash table and get avg reprobes
	createHashTable(size); avgReprobes = 0;
	for(int i = 0; i < size*0.9; i++){
		key = rand();
		value = rand() % 100000 + 1;
		insert(key, value, size);
	}
	avgReprobes = reprobes/(size*0.9);
	printf("90%% load avg insert reprobes = %f\n", avgReprobes);
	reprobes = 0; avgReprobes = 0;
	//search for 10000 numbers in a 90% full hash table and get avg reprobes
	for(int i = 0; i < 10000; i++){
		key = rand();
		value = rand() % 100000 + 1;
		item = search(key, size);
	}
	avgReprobes = reprobes/10000.00;
	printf("90%% load avg search reprobes = %f\n", avgReprobes);
	
   	free(hashArray);
   	free(item);
}