#include "net.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "config.h"
#include "error.h"

void set_fd_nonblock(int fd) {
	int flags = unix_err(fcntl(fd, F_GETFL, 0), "fcntl:getfl");
	unix_err(fcntl(fd, F_SETFL, flags | O_NONBLOCK), "fcntl:setfl");
}

int setup_server_fd(int port) {
	int sockfd = unix_err(socket(AF_INET, SOCK_STREAM, 0), "socket");

	int opt = 1;
	unix_err(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "setsockopt");
	set_fd_nonblock(sockfd);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	unix_err(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)), "bind");
	unix_err(listen(sockfd, SOMAXCONN), "listen");
	return sockfd;
}

void accept_register_clients(int server_fd, int epoll_fd) {
	while (1) {
		int client_fd = accept(server_fd, NULL, 0);
		if (client_fd == -1) {
			// processed all clients
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
				break;
			} else {
				unix_err(client_fd, "accept");
			}
		}

		set_fd_nonblock(client_fd);
		struct epoll_event client_ev = {.events = EPOLLIN | EPOLLET, .data.fd = client_fd};
		unix_err(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev), "epoll_ctl:client");
	}
}

int read_to_end(int fd, char* buf, int len) {
	int ret = 0;
	while (1) {
		int n = read(fd, buf, len);
		if (n == -1) {
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
				break;
			} else {
				unix_err(n, "read");
			}
		} else {
			len -= n;
			ret += n;
		}
	}
	return ret;
}

// update
int write_to_end(int fd, char* buf, int len) { return unix_err(write(fd, buf, len), "write"); }

void handle_http_request(int client_fd, config_t* config, char* request_buf, char* response_buf) {
	if (memcmp(request_buf, HTTP_GET, STR_LEN(HTTP_GET))) {
		write_to_end(client_fd, INVALID_METHOD, STR_LEN(INVALID_METHOD));
	}

	for (int i = STR_LEN(HTTP_GET); i < REQUEST_LEN - 1; i++) {
		if (*(request_buf + i) == ' ') {
			*(request_buf + i) = '\0';
			break;
		}
	}

	char* redirect = hash_map_get(config, request_buf + STR_LEN(HTTP_GET));
	if (!redirect) {
		write_to_end(client_fd, INVALID_ROUTE, STR_LEN(INVALID_ROUTE));
		return;
	}

	int response_len =
	    sprintf(response_buf, "HTTP/1.1 307 Temporary Redirect\r\nLocation: %s\r\nConnection: close\r\n", redirect);
	write_to_end(client_fd, response_buf, response_len);
}
