#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include "p1_process.h"
#include "p1_threads.h"

//Mohammed Haque
//62655407
//4/21/19

int main(int argc, char** argv)
{
	pid_t parent = getpid();
	printf("Main process is created. (pid: %d)\n", parent);
	int num_processes = 0;
	int num_threads = 0;

	std::string class_name[] = {"os", "architecture", "java", "algorithm", "digital-design"};

	// Check the argument and print error message if the argument is wrong
	if(argc == 3 && (atoi(argv[1]) > 0 && atoi(argv[2]) > 0))
	{
		num_processes = atoi(argv[1]);
		num_threads = atoi(argv[2]);
		// Call the process
		get_statistics(class_name, num_processes, num_threads);
	}
	else
	{
		printf("[ERROR] Expecting 2 arguments with integral value greater than zero.\n");
		printf("[USAGE] p1_exec <number of processes> <number of threads>\n");
	}
	
	if(getpid() == parent)	printf("Main process is terminated. (pid: %d)\n", getpid());
	else	printf("Child process is terminated. (pid: %d)\n", getpid());
	return 0;
	
}

