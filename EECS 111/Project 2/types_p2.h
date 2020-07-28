#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
using namespace std;

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2


class Person
{

	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
	void wait(int g);
};


// Class for the fittingroom
// You may need to add more class member variables and functions
class Fittingroom {
	int status;
	int numOfRooms;
	struct timeval t_start;
	struct timeval t_end;

	// You need to define the data structure to
    // save the information of people using the fittingroom
	// You can probebly use Standard Template Library (STL) vector


public:
	Fittingroom(){
		status = EMPTY;

	}

	void woman_wants_to_enter(void);
	void man_wants_to_enter(void);
	void man_leaves(void);
	void woman_leaves(void);

	void setRooms(int data);
	int getRooms(void);

	void setStatus(int data);
	int getStatus(void);

	// You need to use this function to print the Fittingroom's status
	void print_status(void);


	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);







};










#endif
