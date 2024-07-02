#include <stdio.h>
#include <stdlib.h>

int unix_err(int res, char* msg) {
	if (res < 0) {
		perror(msg);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	return res;
}

void* unix_err_ptr(void* res, char* msg) {
	if (res == NULL) {
		perror(msg);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	return res;
}

void app_err(char* msg) {
	fprintf(stderr, "lnk: %s\n", msg);
	exit(EXIT_FAILURE);
}
