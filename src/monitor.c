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

#define mutex_in(X) pthread_mutex_lock(X)
#define mutex_out(X) pthread_mutex_unlock(X)

struct monitor {
	pthread_mutex_t lock;
	struct tlist *urgent;
};

typedef struct monitor *monitor;

struct condition {
	monitor m;
	struct tlist *q;
};

typedef struct condition *condition;

monitor monitor_create(void) {
	monitor m = malloc(sizeof(*m));
	if (m) {
		pthread_mutex_init(&m->lock, NULL);
		m->urgent = NULL;
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

condition condition_create(monitor m) {
	condition c = malloc(sizeof(*c));
	if (c) {
		c->m = m;
		c->q = NULL;
	}
	return c;
}

void condition_destroy(condition c) {
	free(c);
}

void condition_wait(condition c) {
	//printf("wait %p\n",c);
	tlist_enqueue(&c->q, pthread_self());
	monitor_exit(c->m);
	suspend();
}

void condition_signal(condition c) {
	//printf("signal %p\n",c);
	while (!tlist_empty(c->q)) {
		monitor m = c->m;
		pthread_t t = tlist_dequeue(&c->q);
		wakeup(t);
	}
}



// struct Monitor{
//     pthread_t simulation_lock;  //used to stop the simulation from the main program (example for command by the user)
//     sem_t * lock;  //used to block the access at the critical section
//     sem_t * write; //used as a queue for the processes
//     int waiting;
//     int numberOfThreads;
// }
//
// void mon_initialize(struct Monitor *monitor, int numberOfThreads){
//     sem_init(monitor->lock, 0 ,1);
//     sem_init(monitor->write, 0 ,0);
//     monitor->numberOfThreads = numberOfThreads;
//     simulation_lock = 1;
// }
//
//
// void mon_destroy(struct Monitor *monitor){
//
//     sem_destroy(monitor->lock);
//     sem_destroy(monitor->write);
//     free(monitor);
// }
//
// void mon_wait(struct Monitor *monitor){
//
//     sem_wait(monitor->lock);
//     if(waiting == (numberOfThreads-1){
//         waiting = 0;
//         for (int i = 0; i < numberOfThreads-1; i++){
//         sem_post(monitor->write);
//         }
//         sem_post(monitor->lock);
//     }
//     else{
//         waiting++;
//         sem_post(monitor->lock);
//         sem_wait(monitor->write);
//     }
// }
