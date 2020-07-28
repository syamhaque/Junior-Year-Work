#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <queue>
#include "types_p2.h"
using namespace std;

struct threadData{
	int currAmount[2];
	int men, women;
	int numOfRooms;
	int timeToUse;
	queue <Person> *input;
	Fittingroom rooms;
};typedef struct threadData threadData;

void *threadfunc(void *parm);

void *enterRoomthread(void *arg);

void *leaveRoomthread(void *arg);


#endif
