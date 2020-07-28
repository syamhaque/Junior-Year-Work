#ifndef __P1_PROCESS
#define __P1_PROCESS

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


#include "p1_threads.h"

using namespace std;

struct Student{
	double id;
	double grade;
};typedef struct Student Student;

struct threadData{
	int threads;
	double size;
	int part;
	vector<Student> *student;
};typedef struct threadData threadData;

void readFile(vector<Student> &student, string class_name[], int num);

void threadSort(vector<Student> &student, int num_threads);

void outputSorted(vector<Student> &student, string class_name[], int num);

void outputStats(vector<Student> &student, string class_name[], int num);

void get_statistics(std::string class_name[], int num_processes, int num_threads);

#endif
