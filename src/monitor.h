#ifndef MONITOR_H
#define MONITOR_H
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<suspend.h>
#include<semaphore.h>

#define SIMULATION_END 4
#define EXIT_THREAD    3
#define BREAK_BARRIER  2
#define WAIT_FOR_START 1
#define SPIN_AT_BARRIER 0


struct monitor;
typedef struct monitor *monitor;
struct condition;
typedef struct condition *condition;

monitor monitor_create(void);

void monitor_destroy(monitor m);

void monitor_enter(monitor m);

void monitor_exit(monitor m);

condition condition_create(monitor m, int n_thread_max, int n_iteration, int max_iteration);

int condition_check(condition c);

void condition_destroy(condition c);

int condition_current_iteration(condition c);

int condition_max_iteration(condition c);

int condition_threads_waiting(condition c);

void condition_wait(condition c);

void condition_commander_start(condition c);

void condition_signal(condition c);

void condition_commander_signal(condition c);

void simulation_lock_on(condition c);

void simulation_lock_off(condition c);

#endif
