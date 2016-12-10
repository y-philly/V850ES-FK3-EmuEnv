#include "cpu_control/dbg_cpu_thread_control.h"
#include "std_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>



typedef enum {
	THREAD_STATE_RUNNING,
	THREAD_STATE_WAIT,
} DbgCpuThrStateType;
static pthread_mutex_t dbg_mutex;
static pthread_cond_t dbg_cv;
static pthread_cond_t cpu_cv;
static DbgCpuThrStateType dbgthr_state = THREAD_STATE_RUNNING;
static DbgCpuThrStateType cputhr_state = THREAD_STATE_WAIT;

void cputhr_control_init(void)
{
	pthread_mutex_init(&dbg_mutex, NULL);
	pthread_cond_init(&dbg_cv, NULL);
	pthread_cond_init(&cpu_cv, NULL);
	return;

}

void cputhr_control_start(void)
{
	cputhr_state = THREAD_STATE_RUNNING;
	//pthread_create(&thread , NULL , cpu_run , NULL);
}


void cputhr_control_cpu_wait(void)
{
	pthread_mutex_lock(&dbg_mutex);
	cputhr_state = THREAD_STATE_WAIT;
	pthread_cond_wait(&cpu_cv, &dbg_mutex);
	cputhr_state = THREAD_STATE_RUNNING;
	pthread_mutex_unlock(&dbg_mutex);
	return;
}
void cputhr_control_dbg_wait(void)
{
	pthread_mutex_lock(&dbg_mutex);
	dbgthr_state = THREAD_STATE_WAIT;
	pthread_cond_wait(&dbg_cv, &dbg_mutex);
	dbgthr_state = THREAD_STATE_RUNNING;
	pthread_mutex_unlock(&dbg_mutex);
	return;
}

void cputhr_control_dbg_wakeup_cpu_and_wait_for_cpu_stopped(void)
{
	pthread_mutex_lock(&dbg_mutex);
	if (cputhr_state == THREAD_STATE_WAIT) {
		cputhr_state = THREAD_STATE_RUNNING;
		pthread_cond_signal(&cpu_cv);
	}
	while (cputhr_state == THREAD_STATE_RUNNING) {
		pthread_cond_wait(&dbg_cv, &dbg_mutex);
	}
	pthread_mutex_unlock(&dbg_mutex);
	return;
}

void cputhr_control_dbg_waitfor_cpu_stopped(void)
{
	pthread_mutex_lock(&dbg_mutex);
	while (cputhr_state == THREAD_STATE_RUNNING) {
		pthread_cond_wait(&dbg_cv, &dbg_mutex);
	}
	pthread_mutex_unlock(&dbg_mutex);
	return;
}

void cputhr_control_dbg_wakeup_cpu(void)
{
	pthread_mutex_lock(&dbg_mutex);
	pthread_cond_signal(&cpu_cv);
	pthread_mutex_unlock(&dbg_mutex);
}
void cputhr_control_cpu_wakeup_dbg(void)
{
	pthread_mutex_lock(&dbg_mutex);
	pthread_cond_signal(&dbg_cv);
	pthread_mutex_unlock(&dbg_mutex);
}
