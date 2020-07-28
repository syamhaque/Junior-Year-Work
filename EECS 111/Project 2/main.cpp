#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"
using namespace std;

// Mohammed Haque
// 62655407
// 5/14/2019

pthread_cond_t  initial = PTHREAD_COND_INITIALIZER;
pthread_cond_t  leave  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t present = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t size = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;

int main(int argc, char** argv)
{
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);


	pthread_t       tid;
	// pthread_t       tid2;
	int             status = 0;
	int             work = 0;


	
	// Example code for sleep and class usage
	srand(time(NULL));
	int i = 0;
	int g = 0;
	int amountOfEach[2] = {0};
	int currAmountOfEach[2] = {0};
	int waitToGen = 0;
	queue <Person> wait;
	Person p;
	threadData *td = (threadData*)malloc(sizeof(threadData));
	if(argc == 3 && atoi(argv[1]) > 0 && atoi(argv[2]) > 0){
		int numOfPeople = atoi(argv[1]);
		td->numOfRooms = atoi(argv[2]);
		td->input = &wait;
		if(pthread_create(&tid, NULL, enterRoomthread, td))	fprintf(stderr, "Error creating thread\n");
		for(i = 0; i < (numOfPeople*2); i++){
			g = (rand()%2);
			if(g == 0){
				if(amountOfEach[0] >= numOfPeople){
					g = 1;
					amountOfEach[1]++;
				}
				else {
					amountOfEach[0]++;
				}
			}
			else{
				if(amountOfEach[1] >= numOfPeople){
					g = 0;
					amountOfEach[0]++;
				}
				else {
					amountOfEach[1]++;
				}
			}
			p.set_gender(g);
			waitToGen = (rand()%4 + 1);
			usleep(MSEC(waitToGen));
			p.wait(g);
			pthread_mutex_lock(&mutex);
			wait.push(p);
			if(g == 0)	td->currAmount[0]++;
			else	td->currAmount[1]++;
			pthread_cond_signal(&initial);
			pthread_mutex_unlock(&mutex);	
		}
		if(pthread_join(tid, NULL))	fprintf(stderr, "Error joining thread\n");
	}	
	else{
		printf("[ERROR] Expecting 2 arguments with value greater than 0.\n");
		printf("[USAGE] p2_exec <number of processes> <number of threads>\n");
	}

	return 0;
}

