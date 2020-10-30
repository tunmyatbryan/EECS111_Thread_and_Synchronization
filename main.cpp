#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

using namespace std;

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;


queue<Person> male_queue;
queue<Person> female_queue;

Fittingroom fittingroom;

int no_more_people_left;

int main(int argc, char** argv)
{
	int each_gender_cnt = 0, number_of_stalls = 0;
	int number_of_threads = 2;
	int i = 0;
	

	if(argc != 3 || atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 ){
		fprintf(stderr, "[ERROR] Expecting 2 argument, but got (X).\n");
		fprintf(stderr, "[USAGE] ./p2_exec <each_gender_count> <num_stalls>\n");
		return -1;
	}

	each_gender_cnt = atoi(argv[1]);
	number_of_stalls = atoi(argv[2]);

	srand((unsigned) time(NULL));

	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);

	
	fittingroom.set_number_of_stalls(number_of_stalls);
	no_more_people_left = 0;

	generator_queue(each_gender_cnt);

	pthread_t thread_id[number_of_threads];


	pthread_create(&thread_id[1], NULL, leave_t, NULL);
	pthread_create(&thread_id[2], NULL, enter_t, NULL);

	while(i < number_of_threads){
		pthread_join(thread_id[i], NULL);
		i++;
	}

	

	return 0;


}

