#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"
#include "error.h"
#include "hash_map.h"
#include "net.h"
#include "squeue.h"
#include "threading.h"

int main() {
	squeue_t squeue;
	squeue_init(&squeue);

	config_t* config = config_parse(CONFIG_PATH);

	threadpool_t pool;
	threadpool_init(&pool, NUM_THREADS, &squeue, config);

	int epoll_fd = unix_err(epoll_create1(0), "epoll_create");

	int port = 8080;
	int server_fd = setup_server_fd(port);
	struct epoll_event server_ev = {.events = EPOLLIN | EPOLLET, .data.fd = server_fd};
	unix_err(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &server_ev), "epoll_ctl:server");

	struct epoll_event events[MAX_EPOLL_EVENTS];
	while (1) {
		int n_events = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
		for (int i = 0; i < n_events; i++) {
			if (events[i].data.fd == server_fd) {
				// new connection
				accept_register_clients(server_fd, epoll_fd);
			} else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
			           (!(events[i].events & EPOLLIN))) {
				// socket closed
				close(events[i].data.fd);
			} else {
				// epollin (data to read)
				squeue_push(&squeue, events[i].data.fd);
			}
		}
	}

	close(epoll_fd);
	squeue_deinit(&squeue);
}
