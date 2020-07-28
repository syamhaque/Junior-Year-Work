#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  initial;
extern pthread_cond_t  leave;
extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern pthread_mutex_t present;
extern pthread_mutex_t size;
extern struct timeval t_global_start;

void *enterRoomthread(void *arg){

	pthread_t       tid2; 
	struct timeval t_start;
	threadData *td = (threadData*)arg;
	td->men = 0;
	td->women = 0;
	int wasFull = 0;
	int totalRooms = td->numOfRooms;
	td->rooms.setStatus(0);
	td->rooms.setRooms(0);

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&initial, &mutex);
	pthread_mutex_unlock(&mutex);
	while(!td->input->empty()){
		if(td->input->front().get_gender() == 1 && (td->rooms.getStatus() == 1 || td->rooms.getStatus() == 0) && td->rooms.getRooms() < totalRooms){
			if(pthread_create(&tid2, NULL, leaveRoomthread, td))	fprintf(stderr, "Error creating thread\n");
			td->timeToUse = (rand()%8 + 3);
			pthread_mutex_lock(&mutex);
			gettimeofday(&t_start, NULL);
			td->rooms.woman_wants_to_enter();
			td->men = 0;
			td->women++;
			td->currAmount[1]--;
			td->input->pop();
			printf("(Stay %d ms), Status: Total: %lu (Men: %d, Women: %d)\n", td->timeToUse, td->input->size(), td->currAmount[0], td->currAmount[1]);
			printf("[%lu ms][fitting room] (Woman) goes into the fitting room, State is (WomenPresent): Total: %d (Men: %d, Women: %d)\n",
			 get_elasped_time(t_global_start, t_start), td->rooms.getRooms(), td->men, td->women);
			pthread_mutex_unlock(&mutex);
		}
		else if(td->input->front().get_gender() == 0 && (td->rooms.getStatus() == 2 || td->rooms.getStatus() == 0) && td->rooms.getRooms() < totalRooms){
			if(pthread_create(&tid2, NULL, leaveRoomthread, td))	fprintf(stderr, "Error creating thread\n");
			td->timeToUse = (rand()%8 + 3);
			pthread_mutex_lock(&mutex);
			gettimeofday(&t_start, NULL);
			td->rooms.man_wants_to_enter();
			td->women = 0;
			td->men++;
			td->currAmount[0]--;
			td->input->pop();
			printf("(Stay %d ms), Status: Total: %lu (Men: %d, Women: %d)\n", td->timeToUse, td->input->size(), td->currAmount[0], td->currAmount[1]);
			printf("[%lu ms][fitting room] (Man) goes into the fitting room, State is (MenPresent): Total: %d (Men: %d, Women: %d)\n",
			 get_elasped_time(t_global_start, t_start), td->rooms.getRooms(), td->men, td->women);
			pthread_mutex_unlock(&mutex);
		}
		usleep(MSEC(td->timeToUse));
		if(td->rooms.getRooms() >= totalRooms){
			pthread_mutex_lock(&mutex);
			pthread_cond_signal(&leave);
			pthread_cond_wait(&cond, &mutex);
			pthread_mutex_unlock(&mutex);
			wasFull = 1;
			if(pthread_join(tid2, NULL))	fprintf(stderr, "Error joining thread\n");
		}
		pthread_cond_signal(&leave);		
	}
	if(wasFull != 1){
		if(pthread_join(tid2, NULL))	fprintf(stderr, "Error joining thread\n");
	}
}

void *leaveRoomthread(void *arg){

	threadData *td = (threadData*)arg;
	int totalRooms = td->numOfRooms;

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&leave, &mutex);
	pthread_mutex_unlock(&mutex);
	if(td->rooms.getStatus() == 1){
		pthread_mutex_lock(&mutex);
		td->women--;
		td->rooms.woman_leaves();
		if(td->women == 0){
			printf("Status is changed, Status is (empty): Total: %d (Men: %d, Women: %d)\n", td->rooms.getRooms(), td->men, td->women);
			pthread_mutex_lock(&present);
			td->rooms.setStatus(EMPTY);
			pthread_mutex_unlock(&present);
		}
		else	printf("Status is (WomenPresent): Total: %d (Men: %d, Women: %d)\n", td->rooms.getRooms(), td->men, td->women);
		pthread_mutex_unlock(&mutex);
	}
	else if(td->rooms.getStatus() == 2){
		pthread_mutex_lock(&mutex);
		td->men--;
		td->rooms.man_leaves();
		if(td->men == 0){
			printf("Status is changed, Status is (empty): Total: %d (Men: %d, Women: %d)\n", td->rooms.getRooms(), td->men, td->women);
			pthread_mutex_lock(&present);
			td->rooms.setStatus(EMPTY);
			pthread_mutex_unlock(&present);
		}
		else	printf("Status is (MenPresent): Total: %d (Men: %d, Women: %d)\n", td->rooms.getRooms(), td->men, td->women);
		pthread_mutex_unlock(&mutex);
	}

	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}