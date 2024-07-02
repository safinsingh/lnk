#ifndef THREADING_H_
#define THREADING_H_

#include <pthread.h>

#include "config.h"
#include "squeue.h"

typedef struct thread_info {
	squeue_t* squeue;
	config_t* config;
} thread_info_t;

typedef struct threadpool {
	pthread_t* threads;
	thread_info_t thread_info;
} threadpool_t;

void threadpool_init(threadpool_t* pool, int num_threads, squeue_t* squeue, config_t* config);

#endif  // THREADING_H_
