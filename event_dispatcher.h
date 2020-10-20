/*
 * =====================================================================================
 *
 *       Filename:  event_dispatcher.h
 *
 *    Description: This file defines the data structures for Event Dispatcher Design 
 *
 *        Version:  1.0
 *        Created:  10/20/2020 08:47:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Juniper Networks
 *
 * =====================================================================================
 */

#ifndef EVENT_DISPATCHER
#define EVENT_DISPATCHER

#include <stdint.h>
#include <pthread.h>
#include "gluethread/glthread.h"

typedef struct event_dispatcher_ event_dispatcher_t;

typedef void (*event_cbk)(event_dispatcher_t *, void *, uint32_t );

typedef struct task_{

	void *data;
	uint32_t data_size;
	event_cbk ev_cbk;
	glthread_t glue;
} task_t ;
GLTHREAD_TO_STRUCT(glue_to_task,
	task_t, glue);

typedef enum {

	EV_DIS_IDLE,
	EV_DIS_TASK_FIN_WAIT,
} EV_DISPATCHER_STATE;


struct event_dispatcher_{

	pthread_mutex_t ev_dis_mutex;

	glthread_t task_array_head;	
	uint32_t pending_task_count;

	EV_DISPATCHER_STATE ev_dis_state;

	pthread_cond_t ev_dis_cond_wait;
	pthread_t *thread;	

	task_t *current_task;
};

#define EV_DIS_LOCK(ev_dis_ptr)		\
	(pthread_mutex_lock(&((ev_dis_ptr)->ev_dis_mutex)))

#define EV_DIS_UNLOCK(ev_dis_ptr)	\
	(pthread_mutex_unlock(&((ev_dis_ptr)->ev_dis_mutex)))

void
event_dispatcher_init();

void *
event_dispatcher_dispatch_next_task(void *arg);

void
event_dispatcher_schedule_task(
	task_t *task);

task_t *
create_new_task(void *arg,
                uint32_t arg_size,
                event_cbk cbk);

void
event_dispatcher_run();

#endif /* EVENT_DISPATCHER  */
