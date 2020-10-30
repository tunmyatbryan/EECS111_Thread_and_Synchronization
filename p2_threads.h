#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <queue>

#include "types_p2.h"


#define man_wants_to_enter_type 0
#define woman_wants_to_enter_type 1
#define man_enters_type 2
#define woman_enters_type 3
#define man_leaves_type 4
#define woman_leaves_type 5

using namespace std;

void *threadfunc(void *parm);

void *leave_t(void *parm);

void *enter_t(void *parm);

void generator_queue(int each_gender_cnt);

void man_wants_to_enter(Person &person);

void woman_wants_to_enter(Person &person);

void man_enters(Person &person);

void woman_enters(Person &person);

void man_leaves(Person &person);

void woman_leaves(Person &person);


void print_strings(Person &person, int type);

#endif
