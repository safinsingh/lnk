#ifndef NET_H_
#define NET_H_

#include "config.h"

void set_fd_nonblock(int fd);
int setup_server_fd(int port);
void accept_register_clients(int server_fd, int epoll_fd);

int read_to_end(int fd, char* buf, int len);
int write_to_end(int fd, char* buf, int len);

void handle_http_request(int client_fd, config_t* config, char* request_buf, char* response_buf);

#endif  // NET_H_
