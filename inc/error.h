#ifndef ERROR_H_
#define ERROR_H_

#define INVALID_METHOD ("HTTP/1.1 404 Not Found\r\n\r\nInvalid method!\r\n")
#define INVALID_ROUTE ("HTTP/1.1 404 Not Found\r\n\r\nInvalid route!\r\n")

int unix_err(int res, char* msg);
void* unix_err_ptr(void* res, char* msg);
void app_err(char* msg);

#endif  // ERROR_H_
