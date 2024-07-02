#ifndef CONFIG_H_
#define CONFIG_H_

#include "common.h"
#include "hash_map.h"

typedef hash_map_t config_t;

#define SEPARATOR (";")

// minimal line: <from>;<to> (maxlen(from) same as MAX_REDIRECT_LEN)
#define MAX_LINE_LENGTH (STR_LEN(SEPARATOR) + MAX_REDIRECT_LEN * 2)

config_t* config_parse(char* path);

#endif  // CONFIG_H_
