#ifndef SQUEUE_H_
#define SQUEUE_H_

#include <pthread.h>

typedef struct squeue_node {
	// conn fd
	int fd;
	struct squeue_node* next;
} squeue_node_t;

typedef struct squeue {
	squeue_node_t* head;
	squeue_node_t* tail;

	pthread_cond_t not_empty;
	pthread_mutex_t mux;
} squeue_t;

void squeue_init(squeue_t* squeue);
void squeue_push(squeue_t* squeue, int fd);
int squeue_pop_front(squeue_t* squeue);
void squeue_deinit(squeue_t* squeue);

#endif  // SQUEUE_H_
