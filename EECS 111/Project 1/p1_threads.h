#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>
#include <cmath>
#include <vector>
#include<sys/wait.h>
#include <sys/types.h>

#include "p1_process.h"

using namespace std;

struct Student;

struct threadData;

void merge(vector<Student> &student, int l, int m, int r);

void mergesort(vector<Student> &student, int l, int r);

void *merge_sort(void *arg);

#endif
