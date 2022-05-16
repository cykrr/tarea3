#ifndef UTIL_H
#define UTIL_H
#include "list.h"

#ifndef _WIN32
char *_strdup(const char *str);
#endif

//Crea la lista de generos y guarda los mismos al detectarlos
List *strToList(const char *string, const char* delim);

int lower_than_string(void* key1, void* key2);

char* stringToLower(char* words);

#endif