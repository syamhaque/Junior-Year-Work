/**
 *  \file hashing.c
 *  Mohammed Haque 62655407
    2/22/19
 *  \brief Implement your Hashing with open addressing
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
}; typedef struct HashNode HashNode;


HashNode **hashArray; //array to hold hash table
HashNode *item;
int reprobes;

//generate hash function using multiplicaiton method
int hashFn(int key, int size){
	return size * ((key*0.6180339887) - (int)(key*0.6180339887));
}

//create and initialize hash table
void createHashTable(int size){

	hashArray = (HashNode**)malloc(sizeof(HashNode*) * size);
	item = 0;
	reprobes = 0;

	for(int i = 0; i < size; i++){
		hashArray[i] = NULL;
	}
	
	assert(hashArray);

}

//search for key using the hash function
struct HashNode *search(int key, int size){

	int index = hashFn(key, size);
	int counter = 0;
	while(hashArray[index] != NULL){		//while array isn't empty search for key
		counter++;
		if(counter>=size) return NULL;
		if(hashArray[index]->key == key){	//if index returns key then key is found
			return hashArray[index];
		}
		index++;							//iterate thru array
		reprobes++;
		index %= size;
	}

	return NULL;
}

//insert value at index found using hash function and key
void insert(int key, int value, int size){

	int index = hashFn(key, size);

	item = (HashNode*)malloc(sizeof(HashNode));						//allocate memory for item to be inserted

	//set key and value for item to ones going to be inserted
	item->key = key;
	item->value = value;

	while(hashArray[index] != NULL && hashArray[index]->key != -1){	//find position in array that is empty starting from index if index of array is occupied
		index++;
		reprobes++;
		index %= size;
	}
	
	hashArray[index] = item;										//insert item into first empty position of array starting from index
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