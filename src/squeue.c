#include "squeue.h"

#include <stdlib.h>

void squeue_init(squeue_t* squeue) {
	squeue->head = NULL;
	squeue->tail = NULL;

	pthread_mutex_init(&squeue->mux, NULL);
	pthread_cond_init(&squeue->not_empty, NULL);
}

void squeue_push(squeue_t* squeue, int fd) {
	squeue_node_t* node = malloc(sizeof(*node));
	node->fd = fd;
	node->next = NULL;

	pthread_mutex_lock(&squeue->mux);

	// queue empty
	if (squeue->tail == NULL) {
		squeue->head = node;
	} else {
		squeue->tail->next = node;
	}
	squeue->tail = node;

	pthread_cond_signal(&squeue->not_empty);
	pthread_mutex_unlock(&squeue->mux);
}

int squeue_pop_front(squeue_t* squeue) {
	pthread_mutex_lock(&squeue->mux);

	while (squeue->head == NULL) {
		pthread_cond_wait(&squeue->not_empty, &squeue->mux);
	}

	squeue_node_t* node = squeue->head;
	int fd = node->fd;

	squeue->head = squeue->head->next;
	// set queue empty
	if (squeue->head == NULL) {
		squeue->tail = NULL;
	}

	pthread_mutex_unlock(&squeue->mux);
	free(node);
	return fd;
}

void squeue_deinit(squeue_t* squeue) {
	while (squeue->head != NULL) {
		squeue_node_t* tmp = squeue->head;
		squeue->head = squeue->head->next;
		free(tmp);
	}

	pthread_mutex_destroy(&squeue->mux);
	pthread_cond_destroy(&squeue->not_empty);
}
