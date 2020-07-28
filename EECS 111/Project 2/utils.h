#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
using namespace std;
#define MSEC(x) x*1000


long get_elasped_time(struct timeval& start, struct timeval& end);




#endif
