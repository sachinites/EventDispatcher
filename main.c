/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: This file demonstrates the use of Event Dispatcher 
 *
 *        Version:  1.0
 *        Created:  10/20/2020 02:14:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Juniper Networks
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "event_dispatcher.h"

void
compute_square2(event_dispatcher_t *ev_dis, void *arg, uint32_t arg_size){

	int *a = (int *)arg;
	printf("Square of %d = %d\n", *a, (*a) * (*a));	
}

void
compute_square(event_dispatcher_t *ev_dis, void *arg, uint32_t arg_size){

	int *a = (int *)arg;
	printf("Square of %d = %d\n", *a, (*a) * (*a));
	
	a = calloc(1, sizeof(int));
	*a = 10;
	task_t *task_2 	= create_new_task((void *)a, sizeof(*a), compute_square2);

	a = calloc(1, sizeof(int));
	*a = 20;
	task_t *task_3 	= create_new_task((void *)a, sizeof(*a), compute_square2);
	
	event_dispatcher_schedule_task(task_2); 
	event_dispatcher_schedule_task(task_3);
}

int
main(int argc, char **argv){

	event_dispatcher_init();
	event_dispatcher_run();
	//sleep(1);

	int *a = calloc(1, sizeof(int));
	*a = 5;
	task_t *task_1 	= create_new_task((void *)a, sizeof(*a), compute_square);

	a = calloc(1, sizeof(int));
	*a = 6;
	task_t *task_2 	= create_new_task((void *)a, sizeof(*a), compute_square);

	a = calloc(1, sizeof(int));
	*a = 9;
	task_t *task_3 	= create_new_task((void *)a, sizeof(*a), compute_square);

	event_dispatcher_schedule_task(task_1); 
	//event_dispatcher_schedule_task(task_2); 
	//event_dispatcher_schedule_task(task_3);

	pause(); 
	return 0;
}




