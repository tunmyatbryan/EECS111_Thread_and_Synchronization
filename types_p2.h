#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define EMPTY        0
#define WOMANPRESENT 1
#define MANPRESENT   2
#define FULL         3

#define MALE   0
#define FEMALE 1

using namespace std;

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
	void set_gender_str(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);

	long get_stay_time();

	int ready_to_leave(void);

	void start(void);
	void complete(void);

	
};


// Class for the fittingroom
// You may need to add more class member variables and functions
class Fittingroom {
	int status;
	int number_of_stalls;

	
	// You need to define the data structure to
    	// save the information of people using the fittingroom
	// You can probebly use Standard Template Library (STL) vector

	Person *changing_person_info;
	
	char *busy_stall_info;


	int occupied_stall_count;

	char gender_flag;

	


public:
	Fittingroom();

	~Fittingroom();

	
	void add_person_to_stall(Person &p);
	void remove_person_from_stall(int person_count);

	

	void set_number_of_stalls(int number);

	int get_fittingroom_status(void);

	string fittingroom_status_print(void);

	int get_occupied_stall_count(void);

	int men_changing_count(void);

	int women_changing_count(void);


	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);

	int remove_person(void);




};










#endif
