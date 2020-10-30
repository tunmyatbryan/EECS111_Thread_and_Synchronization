#include "types_p2.h"
#include "utils.h"
#include "p2_threads.h"



void Person::set_gender(int data) { 
	gender = data; 
}


int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }

long Person::get_stay_time() { return time_to_stay_ms; }

int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}


void Person::set_gender_str(int data) {

	if(data == MALE){
		str_gender == "Man";
	}
	else if(data == FEMALE){
		str_gender == "Woman";
	}
}


void Person::start(void) {
	gettimeofday(&t_start, NULL);
}

void Person::complete(void) {
	gettimeofday(&t_end, NULL);
}

Person::Person() {
	gettimeofday(&t_create, NULL);
}



Fittingroom::Fittingroom(){
	changing_person_info = NULL;
	busy_stall_info = NULL;
	status = EMPTY;
}


Fittingroom::~Fittingroom(){
	if(changing_person_info != NULL){
		delete[] changing_person_info;
	}

	if(busy_stall_info != NULL){
		delete[] busy_stall_info;
	}

	changing_person_info = NULL;
	busy_stall_info = NULL;
}


int Fittingroom::get_fittingroom_status(void){
	return status;
}

void Fittingroom::add_person_to_stall(Person &person){
	int i = 0;

	while(i < number_of_stalls){

		if(busy_stall_info[i] == 0 && status != FULL && busy_stall_info != NULL && changing_person_info != NULL){

			changing_person_info[i] = person;
			busy_stall_info[i] = 1;
			occupied_stall_count = occupied_stall_count + 1;

			i = number_of_stalls + 1;

		}
		i++;
	}

}


void Fittingroom::remove_person_from_stall(int person_count){

	if( status != EMPTY && changing_person_info != NULL && busy_stall_info != NULL && person_count >= 0 && person_count < number_of_stalls ){

			busy_stall_info[person_count] = 0;
			occupied_stall_count = occupied_stall_count - 1;

	}

}



// Call by reference
// This is just an example. You can implement any function you need
void Fittingroom::add_person(Person &person) {
	
	int gender_type = person.get_gender();


	if((status == EMPTY) && (status != FULL) && (gender_type == FEMALE)){
		person.start();
		add_person_to_stall(person);
		gender_flag = FEMALE;
		status = WOMANPRESENT;
	}

	else if((status == EMPTY) && (status != FULL) && (gender_type == MALE)){
		person.start();
		add_person_to_stall(person);
		gender_flag = MALE;
		status = MANPRESENT;
	}

	else if( (status != FULL) && (status == WOMANPRESENT) && (gender_type == FEMALE) ){

		person.start();
		add_person_to_stall(person);
		gender_flag = FEMALE;
		status = WOMANPRESENT;

	}

	else if( (status != FULL) && (status == MANPRESENT) && (gender_type == MALE) ){
		
		person.start();
		add_person_to_stall(person);
		gender_flag = MALE;
		status = MANPRESENT;

	}


	if((status == FULL) || (occupied_stall_count == number_of_stalls)){
		status = FULL;
	}

	
	if((status != FULL) && (gender_type == MALE)){
		man_enters(person);
		gender_flag = MALE;
		status = MANPRESENT;
	}
	else if((status != FULL) && (gender_type == FEMALE)){
		woman_enters(person);
		gender_flag = FEMALE;
		status = WOMANPRESENT;
	}


}


int Fittingroom::remove_person(void){

	int remove_flag = 0;
	int i = 0;
	int ready_to_leave_flag;

	int stall_curr_gender_type;

	while(i < number_of_stalls){

		ready_to_leave_flag = changing_person_info[i].ready_to_leave();
	
		if(ready_to_leave_flag == 1 && busy_stall_info[i] ){

			stall_curr_gender_type = changing_person_info[i].get_gender();

			changing_person_info[i].complete();
			remove_person_from_stall(i);

			if(occupied_stall_count == 0){
				status = EMPTY;
			}
			else if(status != EMPTY && status == FULL) {

				if(stall_curr_gender_type == MALE){
					status = MANPRESENT;
				}
				else if(stall_curr_gender_type == FEMALE){
					status = WOMANPRESENT;
				}

			}

			if(stall_curr_gender_type == MALE){
				man_leaves(changing_person_info[i]);
			}
			else if(stall_curr_gender_type == FEMALE){
				woman_leaves(changing_person_info[i]);
			}

			remove_flag = 1;
		}

		i++;
	}

	return remove_flag;

}



void Fittingroom::set_number_of_stalls(int num_stalls){

	int i = 0;
	number_of_stalls = num_stalls;
	changing_person_info = new Person[number_of_stalls];
	busy_stall_info = new char[number_of_stalls];

	while(i < number_of_stalls){
		busy_stall_info[i] = 0;
		i++;
	}

	occupied_stall_count = 0;
}





string Fittingroom::fittingroom_status_print(void){

	if(status == EMPTY){
		return "EMPTY";
	}

	if(status == WOMANPRESENT){
		return "WOMANPRESENT";
	}

	if(status == MANPRESENT){
		return "MANPRESENT";

	}

	if(status == FULL){
		return "FULL";
	}
}


int Fittingroom::get_occupied_stall_count(void){
	return occupied_stall_count;
}


int Fittingroom::men_changing_count(void){

	int result;

	if(gender_flag == MALE){
		result = occupied_stall_count;
	}
	else{
		result = 0;
	}

	return result;
}

int Fittingroom::women_changing_count(void){

	int result;

	if(gender_flag == FEMALE){
		result = occupied_stall_count;
	}
	else{
		result = 0;
	}

	return result;

}





