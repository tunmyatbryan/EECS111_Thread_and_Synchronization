#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;


extern queue<Person> male_queue;
extern queue<Person> female_queue;

extern int no_more_people_left;

extern Fittingroom fittingroom;

extern struct timeval t_global_start;

void *threadfunc(void *parm)
{

	int status;
	printf(" [Thread] Start\n");

	printf(" [Thread] Locks\n");
	status = pthread_mutex_lock(&mutex);

    printf(" [Thread] Blocked\n");
    status = pthread_cond_wait(&cond, &mutex);

	printf(" [Thread] Starts again.\n");
	for (int i=0; i<3; i++) {
		printf(" [Thread] Complete thread after (%d) seconds\n", (3-i));
		usleep(MSEC(1000));
	}

	printf(" [Thread] Unlocks\n");
	status = pthread_mutex_unlock(&mutex);
	printf(" [Thread] Complete\n");
}





void *leave_t(void *parm){

	int status;
	int fittingroom_flag;

	while(1){

		fittingroom_flag = fittingroom.get_fittingroom_status();

		if(male_queue.empty() && female_queue.empty() && (fittingroom_flag == EMPTY) && no_more_people_left == 1){
			break;
		}

		status = pthread_mutex_lock(&mutex);

		
		if(fittingroom.remove_person()){
			pthread_cond_signal(&cond);
		}

		status = pthread_mutex_unlock(&mutex);
	

	}

	return NULL;

}


void *enter_t(void *parm){

	int status, fittingroom_flag;
	int random_wait_time;
	bool male_queue_status;
	bool female_queue_status;
	int male_queue_size, female_queue_size;
	Person person;

	while(1){

		fittingroom_flag = fittingroom.get_fittingroom_status();

		if(male_queue.empty() && female_queue.empty() && (fittingroom_flag == EMPTY) && no_more_people_left == 1){
			break;
		}

		random_wait_time = ((rand() % 5) + 1);

		status = pthread_mutex_lock(&mutex);


		if(fittingroom_flag == FULL){
			pthread_cond_wait(&cond, &mutex);
		}

		
		male_queue_status = male_queue.empty();
		female_queue_status = female_queue.empty();
		male_queue_size = male_queue.size();
		female_queue_size = female_queue.size();


		if(fittingroom_flag == MANPRESENT && male_queue_status == false){
			
				person = male_queue.front();
				male_queue.pop();
				fittingroom.add_person(person);
			
		}

		else if(fittingroom_flag == WOMANPRESENT && female_queue_status == false){
			
				person = female_queue.front();
				female_queue.pop();
				fittingroom.add_person(person);
			
		}

		else if(fittingroom_flag == EMPTY){

			if(male_queue_status == false || female_queue_status == false){

				if( female_queue_size > male_queue_size ){
				
					person = female_queue.front();
					female_queue.pop();
					fittingroom.add_person(person);

				}

				else{
					person = male_queue.front();
					male_queue.pop();
					fittingroom.add_person(person);
				}

			}


		}

		status = pthread_mutex_unlock(&mutex);
		
		usleep(MSEC(random_wait_time));

	}

	return NULL;


}


void generator_queue(int each_gender_cnt){
	int status;
	int total_gender_cnt, male_count = 0, female_count = 0;
	int i = 0;
	int random_gender = 0;
	int random_stay_time = 0;
	int random_wait_time = 0;
	Person person;

	total_gender_cnt = 2 * each_gender_cnt;

	while(i < total_gender_cnt){
		
		random_gender = rand() % 2;
		random_stay_time = ((rand() % 10) + 3);
		random_wait_time = ((rand() % 5) + 1);

		if(male_count == each_gender_cnt){
			person.set_gender(FEMALE);
			person.set_gender_str(FEMALE);
			female_count = female_count + 1;
			person.set_order(female_count);
		}

		else if(female_count == each_gender_cnt){
			person.set_gender(MALE);
			person.set_gender_str(MALE);
			male_count = male_count + 1;
			person.set_order(male_count);
		}

		else{
			
			person.set_gender(random_gender);
			person.set_gender_str(random_gender);

			if(person.get_gender() == MALE){
				male_count = male_count + 1;
				person.set_order(male_count);
			}

			else if(person.get_gender() == FEMALE){
				female_count = female_count + 1;
				person.set_order(female_count);
			}


		}

		

		person.set_time(random_stay_time);
	


		if(person.get_gender() == MALE){
			male_queue.push(person);
			man_wants_to_enter(person);
		}

		else if(person.get_gender() == FEMALE){
			female_queue.push(person);
			woman_wants_to_enter(person);
		}

		usleep(MSEC(random_wait_time));
		
		i++;

	}

	no_more_people_left = 1;

}


void man_wants_to_enter(Person &person){

	print_strings(person, man_wants_to_enter_type);

}


void woman_wants_to_enter(Person &person){

	print_strings(person, woman_wants_to_enter_type);

}


void man_enters(Person &person){

	print_strings(person, man_enters_type);

}


void woman_enters(Person &person){

	print_strings(person, woman_enters_type);

}


void man_leaves(Person &person){

	print_strings(person, man_leaves_type);

}


void woman_leaves(Person &person){

	print_strings(person, woman_leaves_type);

}



void print_strings(Person &person, int type){

	int total_size = male_queue.size() + female_queue.size();

	int male_queue_size, female_queue_size;

	long stay_time = person.get_stay_time();


	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	male_queue_size = male_queue.size();
	female_queue_size = female_queue.size();


	//print for man_wants_to_enter function
	if( type == man_wants_to_enter_type){

		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Input] A person (Man) goes into the queue \n";



	}

	//print for woman_wants_to_enter function
	else if(type == woman_wants_to_enter_type){

		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Input] A person (Woman) goes into the queue \n";


	}


	//print for man_etners function
	else if( type == man_enters_type){

		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Queue] Send (Man) into the fitting room (Stay "<< stay_time << " ms), Queue Status: Total: " << total_size << " (Men: " << male_queue_size << ", Women: " << female_queue_size << ")\n";


		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Fitting Room] (Man) goes into the fitting room, State is (MenPresent): Total: " << fittingroom.get_occupied_stall_count() << " (Men: " << fittingroom.men_changing_count() << ", Women: " << fittingroom.women_changing_count() << ")\n";


	}

	//print for woman_enters function
	else if(type == woman_enters_type){

		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Queue] Send (Woman) into the fitting room (Stay "<< stay_time << " ms), Queue Status: Total: " << total_size << " (Men: " << male_queue_size << ", Women: " << female_queue_size << ")\n";


		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Fitting Room] (Woman) goes into the fitting room, State is (WomenPresent): Total: " << fittingroom.get_occupied_stall_count() << " (Men: " << fittingroom.men_changing_count() << ", Women: " << fittingroom.women_changing_count() << ")\n";


	}

	//print for man_leaves function
	else if(type == man_leaves_type){

		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Fitting Room] (Man) left the fitting room. State is changed, Status is ("<< fittingroom.fittingroom_status_print() << "): Total: " << fittingroom.get_occupied_stall_count() << " (Men: " << fittingroom.men_changing_count() << ", Women: " << fittingroom.women_changing_count() << ")\n";

	}

	//print for woman_leaves function
	else if(type == woman_leaves_type){

		cout<< "[" << get_elasped_time(t_global_start, t_curr) << " ms] [Fitting Room] (Woman) leaves the fitting room. State is changed, Status is ("<< fittingroom.fittingroom_status_print() << "): Total: " << fittingroom.get_occupied_stall_count() << " (Men: " << fittingroom.men_changing_count() << ", Women: " << fittingroom.women_changing_count() << ")\n";


	}



}




