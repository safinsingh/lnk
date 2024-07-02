#ifndef COMMON_H_
#define COMMON_H_

#define NUM_THREADS 16
#define MAX_EPOLL_EVENTS 4096
#define CONFIG_PATH ("lnk.cfg")

// convenience fn.
#define STR_LEN(E) (sizeof(E) - 1)

// extra space for parsing
#define REQUEST_LEN 128
#define RESPONSE_LEN 512

#define HTTP_GET ("GET /")
// minimal request: GET /redir (not incl. /)
#define MAX_REDIRECT_LEN (REQUEST_LEN - STR_LEN("/") - STR_LEN(HTTP_GET))

#endif  // COMMON_H_
