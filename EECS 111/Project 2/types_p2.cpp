#include "types_p2.h"
#include "utils.h"

extern pthread_cond_t  initial;
extern pthread_cond_t  leave;
extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern pthread_mutex_t present;
extern pthread_mutex_t size;
extern struct timeval t_global_start;

void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }
int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}

void Person::start(void) {
	gettimeofday(&t_start, NULL);
	printf("(%lu)th person enters the fittingroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}

void Person::complete(void) {
	gettimeofday(&t_end, NULL);
	printf("(%lu)th person comes out of the fittingroom: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
	printf(" - (%lu) milliseconds after using the fittingroom\n", get_elasped_time(t_start, t_end));
}

void Person::wait(int g){
	gettimeofday(&t_start, NULL);
	if(g == 0)
		printf("[%lu ms][Input] A person (Man) goes into the queue\n", get_elasped_time(t_create, t_start));
	else
		printf("[%lu ms][Input] A person (Woman) goes into the queue\n", get_elasped_time(t_create, t_start));
}

Person::Person() {
	gettimeofday(&t_create, NULL);
}

void Fittingroom::setRooms(int data){ numOfRooms = data; }
int Fittingroom::getRooms(void){ return numOfRooms; }

void Fittingroom::setStatus(int data){ status = data; }
int Fittingroom::getStatus(void){ return status; }

void Fittingroom::woman_wants_to_enter(void){

	gettimeofday(&t_start, NULL);
	pthread_mutex_lock(&present);
	setStatus(WOMENPRESENT);
	pthread_mutex_unlock(&present);
	printf("[%lu ms][Queue] Send (Woman) into the fitting room ", get_elasped_time(t_global_start, t_start));

	pthread_mutex_lock(&size);
	setRooms(getRooms()+1);
	pthread_mutex_unlock(&size);
	
}
void Fittingroom::man_wants_to_enter(void){

	gettimeofday(&t_start, NULL);
	pthread_mutex_lock(&present);
	setStatus(MENPRESENT);
	pthread_mutex_unlock(&present);

	printf("[%lu ms][Queue] Send (Man) into the fitting room ", get_elasped_time(t_global_start, t_start));

	pthread_mutex_lock(&size);
	setRooms(getRooms()+1);
	pthread_mutex_unlock(&size);

}
void Fittingroom::woman_leaves(void){

	gettimeofday(&t_end, NULL);

	printf("[%lu ms][fitting room] (Woman) left the fitting room. ", get_elasped_time(t_global_start, t_end));

	pthread_mutex_lock(&size);
	setRooms(getRooms()-1);
	pthread_mutex_unlock(&size);

}
void Fittingroom::man_leaves(void){

	gettimeofday(&t_end, NULL);

	printf("[%lu ms][fitting room] (Man) left the fitting room. ", get_elasped_time(t_global_start, t_end));

	pthread_mutex_lock(&size);
	setRooms(getRooms()-1);
	pthread_mutex_unlock(&size);

}
// You need to use this function to print the Fittingroom's status
void Fittingroom::print_status(void) {
	printf("Print fittingroom status\n");
}


// Call by reference
// This is just an example. You can implement any function you need
void Fittingroom::add_person(Person& p) {
	// Do nothing;
}



