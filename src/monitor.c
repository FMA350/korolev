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
};

typedef struct monitor *monitor;

struct condition {
	int lock;
	int value;

	monitor m;
	struct tlist *q;
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

condition condition_create(monitor m, int conditionValue, int lock) {
	condition c = malloc(sizeof(*c));
	if (c) {
		c->m = m;
		c->q = NULL;
		c->value = conditionValue;
		c->lock  = lock;
	}
	return c;
}

void condition_destroy(condition c) {
	free(c);
}

int condition_check(condition c, int checkvalue){
	if((c->value == checkvalue)&&(!c->lock)){
		return 1;
	}
	return 0;
}

void condition_wait(condition c) {
	//printf("wait %p\n",c);
	c->value++;
	tlist_enqueue(&c->q, pthread_self());
	monitor_exit(c->m);
	suspend();
}

void condition_signal(condition c) {
	//printf("signal %p\n",c);
	// if(!c->lock){
		while (!tlist_empty(c->q)) {
			c->value = 0;
			pthread_t t = tlist_dequeue(&c->q);
			wakeup(t);
		}
	}
	// else{
	// 	c->value++;
	// 	tlist_enqueue(&c->q, pthread_self());
	// 	monitor_exit(c->m);
	// 	suspend();
	// }
// }

void simulation_lock_on(condition c){
	c->lock = 1;

}

void simulation_lock_off(condition c){
	c->lock = 0;

}
