/*
 * =====================================================================================
 *
 *       Filename:  main_pkt_q.c
 *
 *    Description: This file demonstrates the use of pkt Q tasks 
 *
 *        Version:  1.0
 *        Created:  10/21/2020 10:46:38 AM
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

static pkt_q_t pkt_queue1;
static pkt_q_t pkt_queue2;

static void
pkt_reciever1(void *_pkt, uint32_t pkt_size) {

	static int i = 1;
	char *pkt  = (char *)task_get_next_pkt(&pkt_size);

	if(!pkt) {
		return;
	}

	for( ; pkt;
		 pkt = (char *)task_get_next_pkt(&pkt_size)) {

		printf("%s(%d) : pkt recvd : %s\n", __FUNCTION__, i, pkt);
		i++;
	}
}

static void
pkt_reciever2(void *_pkt, uint32_t pkt_size) {

	static int i = 1;
	char *pkt  = (char *)task_get_next_pkt(&pkt_size);

	if(!pkt) {
		return;
	}

	for( ; pkt;
		 pkt = (char *)task_get_next_pkt(&pkt_size)) {

		printf("%s(%d) : pkt recvd : %s\n", __FUNCTION__, i, pkt);
		i++;
	}
}

int
main(int argc, char **argv){

	/*  init the event dispatcher */
	event_dispatcher_init();

	/*  init all pkt Queues */
	init_pkt_q(&pkt_queue1, pkt_reciever1);
	init_pkt_q(&pkt_queue2, pkt_reciever2);

	event_dispatcher_run();

	/* prepare the pkt */
	int i = 0;
	char pkt1[] = "This is first pkt";
	char pkt2[] = "This is second pkt";
	for(; i < 5; i++){
		pkt_q_enqueue(&pkt_queue1, pkt1, sizeof(pkt1));
		pkt_q_enqueue(&pkt_queue1, pkt1, sizeof(pkt1));
		pkt_q_enqueue(&pkt_queue2, pkt2, sizeof(pkt2));
		pkt_q_enqueue(&pkt_queue1, pkt1, sizeof(pkt1));
		pkt_q_enqueue(&pkt_queue1, pkt1, sizeof(pkt1));
		pkt_q_enqueue(&pkt_queue2, pkt2, sizeof(pkt2));
	}
	pause();	
}
