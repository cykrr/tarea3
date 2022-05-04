#include "util.h"
#include <string.h>

#ifndef WIN32
    char * _strdup(const char * str) {
        char * aux = (char *)malloc(strlen(str) + 1);
        strcpy(aux, str);
        return aux;
    }
#endif


//Crea la lista de generos y guarda los mismos al detectarlos
List *strToList(const char *string, const char* delim){
    List *list = listCreate();

    char *aux = _strdup(string);
    char *token = strtok(aux, delim);
    while (token != NULL)
    {
        listPushBack(list, token);
        token = strtok(NULL, delim);
    }  
     
    return list;
}
