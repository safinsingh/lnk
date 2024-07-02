#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "hash_map.h"

config_t* config_parse(char* path) {
	FILE* fp = unix_err_ptr(fopen(path, "r"), "fopen");

	config_t* config = malloc(sizeof(*config));
	hash_map_init(config);

	char line_buf[MAX_LINE_LENGTH];
	while (fgets(line_buf, sizeof(line_buf), fp)) {
		// null-term line
		line_buf[strcspn(line_buf, "\n")] = '\0';

		char* delimiter_pos = strchr(line_buf, ';');
		if (delimiter_pos) {
			*delimiter_pos = '\0';
			char* key = line_buf;
			char* value = delimiter_pos + 1;
			hash_map_insert(config, strdup(key), strdup(value));
		} else {
			app_err("failed to parse config");
		}
	}

	return config;
}
