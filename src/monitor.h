#ifndef MONITOR_H
#define MONITOR_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<semaphore.h>

struct monitor;
typedef struct monitor *monitor;
struct condition;
typedef struct condition *condition;

monitor monitor_create(void);

void monitor_destroy(monitor m);

void monitor_enter(monitor m);

void monitor_exit(monitor m);

condition condition_create(monitor m, int condition, int lock);

int condition_check(condition c, int checkvalue);

void condition_destroy(condition c);

void condition_wait(condition c);

void condition_signal(condition c);

void simulation_lock_on(condition c);

void simulation_lock_off(condition c);

#endif
