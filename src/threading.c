#include "threading.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "config.h"
#include "net.h"
#include "squeue.h"

void* thread_init(void* arg) {
	thread_info_t* thread_info = (thread_info_t*)arg;

	char request_buf[REQUEST_LEN];
	char response_buf[RESPONSE_LEN];

	while (1) {
		memset(request_buf, 0, sizeof(request_buf));
		memset(response_buf, 0, sizeof(response_buf));

		int client_fd = squeue_pop_front(thread_info->squeue);
		read_to_end(client_fd, request_buf, sizeof(request_buf));
		handle_http_request(client_fd, thread_info->config, request_buf, response_buf);

		close(client_fd);
	}
}

void threadpool_init(threadpool_t* pool, int num_threads, squeue_t* squeue, config_t* config) {
	pool->threads = malloc(num_threads * sizeof(pthread_t));
	pool->thread_info.config = config;
	pool->thread_info.squeue = squeue;

	for (int i = 0; i < num_threads; i++) {
		pthread_create(&pool->threads[i], NULL, thread_init, &pool->thread_info);
	}
}
