#include "util.h"
#include <string.h>

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

//Crea la lista de generos y guarda los mismos al detectarlos
List *strToList(const char *string, const char delim){
    List *list = listCreate();

    char *aux = _strdup(string);
    char *token = strtok(aux, &delim);

    int flag = 0;
    while(token != NULL){
        if (flag) listPushBack(list, token+1);
        else listPushBack(list, token);
        flag = 1;

        token = strtok(NULL, &delim);
    }
        
    return list;
}