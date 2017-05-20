/*
 * sm: educational resources to teach concurrent programming
 * Copyright (C) 2016  Renzo Davoli, University of Bologna
 *
 * implementation of monitors
 *
 * sm is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 * Modified by Francesco Maria Moneta for the use in the Korolev simulation program
 * (francesco.moneta@studio.unibo.it)
 * (francesco.moneta@gmail.com)
 *
 */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<monitor.h>
#include<tlist.h>
#include <signal.h>
#include "various.h"

#define mutex_in(X) pthread_mutex_lock(X)
#define mutex_out(X) pthread_mutex_unlock(X)

struct monitor {
	pthread_mutex_t lock;
};

typedef struct monitor *monitor;

struct condition {
	int lock; //simple boolean lock
	int n_thread_waiting; //number of thread waiting in the queue
	int n_thread_max;	  //max number of thread that may be waiting
	//in the queue. When n_thread_waiting = n_thread_max the barrier is released.

	int n_iteration; //how many times the current condition has been signaled
	int max_iteration; //how many times this condition may be signaled before blocking.

	monitor m;
	struct tlist *q;
	struct tlist *commander;
};



typedef struct condition *condition;

monitor monitor_create(void) {
	monitor m = malloc(sizeof(*m));
	if (m) {
		pthread_mutex_init(&m->lock, NULL);
	}
	return m;
}

void monitor_destroy(monitor m) {
	pthread_mutex_destroy(&m->lock);
	free(m);
}

void monitor_enter(monitor m) {
	//printf("enter %p\n",m);
	mutex_in(&m->lock);
}

void monitor_exit(monitor m) {
	//printf("exit %p\n",m);
		mutex_out(&m->lock);
}

condition condition_create(monitor m, int n_thread_max, int n_iteration, int max_iteration){
	condition c = malloc(sizeof(*c));
	if (c) {
		c->lock  			= 1;
		c->n_thread_waiting = 0; //number of thread waiting in the queue
		c->n_thread_max 	= n_thread_max;	  //max number of thread that may be waiting
		//in the queue. When n_thread_waiting = n_thread_max the barrier is released.
		c->n_iteration 	= n_iteration; //how many times the current condition has been signaled //FIXME
		c->max_iteration 	=  max_iteration; //how many times this condition may be signaled before blocking.

		c->m = m;
		c->q = NULL;
	}
	return c;
}

void condition_destroy(condition c) {
	free(c);
}

int condition_check(condition c){
	if(c->n_iteration >= c->max_iteration){
		//if code gets here, simulation is over. c->n_thread_waiting is used
		//to count how many threads left already
		if(c->n_thread_waiting >= c->n_thread_max){
			return SIMULATION_END;
		}
		else{
			c->n_thread_waiting++;
			return EXIT_THREAD;
		}
	}
	else if(c->n_thread_waiting >= c->n_thread_max){
		//checks if this thread is the last to get to this condition (barrier), if so:
		if(!c->lock){
			//not all iterations were done, and no lock is in place
			return BREAK_BARRIER;
		}
		else{
			//if lock is on
			return WAIT_FOR_START;
		}
	}
	else{
		//if instead I am not the last to get to this barrier:
		return SPIN_AT_BARRIER;
	}
}

int condition_current_iteration(condition c){
	return c->n_iteration;
}

int condition_max_iteration(condition c){
	return c->max_iteration;
}

int condition_threads_waiting(condition c){
	return c->n_thread_waiting;
}

void condition_wait(condition c) {
	//printf("wait %p\n",c);
	c->n_thread_waiting++;
	tlist_enqueue(&c->q, pthread_self());
	monitor_exit(c->m);
	suspend();
}

void condition_commander_start(condition c){
	//wake up all the waiting threads
	//used to avoid busy waiting of the commander
	tlist_enqueue(&c->commander, pthread_self());
	// c->n_thread_waiting = 0;
	// while (!tlist_empty(c->q)) {
	// 	pthread_t t = tlist_dequeue(&c->q);
	// 	wakeup(t);
	// }
	//release the lock
	monitor_exit(c->m);
	suspend();
}

void condition_kill_waiting_threads(condition c){
	// while (!tlist_empty(c->q)) {
	// 	printf("killing one\n");
	// 	pthread_t t = tlist_dequeue(&c->q);
	// 	pthread_kill(t, 15);
	// }
	// 切腹
}

void condition_signal(condition c) {
		c->n_iteration++;
		c->n_thread_waiting = 0;
		while (!tlist_empty(c->q)) {
			pthread_t t = tlist_dequeue(&c->q);
			wakeup(t);
		}
	}

void condition_commander_signal(condition c){
	pthread_t t = tlist_dequeue(&c->commander);
	wakeup(t);
}

void simulation_lock_on(condition c){
	c->lock = 1;

}

void simulation_lock_off(condition c){
	c->lock = 0;

}
