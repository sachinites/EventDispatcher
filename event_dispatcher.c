/*
 * =====================================================================================
 *
 *       Filename:  event_dispatcher.c
 *
 *    Description: This file defines the routine to imeplement Event Dispatcher
 *
 *        Version:  1.0
 *        Created:  10/20/2020 09:01:49 AM
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
#include <memory.h>
#include <assert.h>
#include <unistd.h>
#include "event_dispatcher.h"

static event_dispatcher_t ev_dis;

void
event_dispatcher_init(){

	pthread_mutex_init(&ev_dis.ev_dis_mutex, NULL);
	init_glthread(&ev_dis.task_array_head);
	ev_dis.pending_task_count = 0;
	
	ev_dis.ev_dis_state = EV_DIS_IDLE;

	pthread_cond_init(&ev_dis.ev_dis_cond_wait, NULL);
	ev_dis.thread = NULL;
	ev_dis.current_task = NULL;
}

void
event_dispatcher_schedule_task(
	task_t *task){

	assert (IS_GLTHREAD_LIST_EMPTY(&task->glue));

	EV_DIS_LOCK(&ev_dis);

	glthread_add_next(&ev_dis.task_array_head, &task->glue);
	
	ev_dis.pending_task_count++;

	if (ev_dis.ev_dis_state == EV_DIS_IDLE) {

		pthread_cond_signal(&ev_dis.ev_dis_cond_wait);
	}

	EV_DIS_UNLOCK(&ev_dis);
}

void *
event_dispatcher_dispatch_next_task(void *arg) {

	task_t *task;
	glthread_t *first_node;

	EV_DIS_LOCK(&ev_dis);

	if (ev_dis.ev_dis_state == EV_DIS_TASK_FIN_WAIT){
		EV_DIS_UNLOCK(&ev_dis);
		return 0;
	}

	while(1) {
		//sleep(1);	
		first_node = dequeue_glthread_first(&ev_dis.task_array_head);

		if(!first_node) {
			ev_dis.ev_dis_state = EV_DIS_IDLE;
			printf("EVE DIS moved to IDLE STATE\n");
			pthread_cond_wait(&ev_dis.ev_dis_cond_wait,
					&ev_dis.ev_dis_mutex);
		}
		else {
			ev_dis.pending_task_count--;
			task = glue_to_task(first_node);
			ev_dis.current_task = task;
			
			if(ev_dis.ev_dis_state != EV_DIS_TASK_FIN_WAIT){
				ev_dis.ev_dis_state = EV_DIS_TASK_FIN_WAIT;
				printf("EVE DIS moved to EV_DIS_TASK_FIN_WAIT\n");
			}
			EV_DIS_UNLOCK(&ev_dis);
			task->ev_cbk(&ev_dis, task->data, task->data_size);
			EV_DIS_LOCK(&ev_dis);
			ev_dis.current_task = NULL;
		}
	}
	return 0;
}

task_t *
create_new_task(void *arg,
				uint32_t arg_size,
				event_cbk cbk){

	task_t *task = calloc(1, sizeof(task_t));
	task->data = arg;
	task->data_size = arg_size;
	task->ev_cbk = cbk;
	init_glthread(&task->glue);
	return task;
}

void
event_dispatcher_run(){

	pthread_attr_t attr;
	pthread_t *event_dis_thread;
	
	event_dis_thread = calloc(1, sizeof(pthread_t));
	ev_dis.thread = event_dis_thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(event_dis_thread, &attr,
					event_dispatcher_dispatch_next_task,
					NULL);
}
