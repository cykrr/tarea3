#include "util.h"
#include <string.h>
#include <ctype.h>

#ifndef _WIN32
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

int lower_than_string(void* key1, void* key2){
    char* k1=(char*) key1;
    char* k2=(char*) key2;
    if(strcmp(k1,k2)<0) return 1;
    return 0;
}

char* stringToLower(char* words)
{
    int i = 0;
    while (words[i] != '\0')
    {
        words[i] = tolower(words[i]);
        i++;
    }
    return words;
}

int hayQueEliminar(char c, char* string_chars)
{
    for(int i=0 ; i<strlen(string_chars) ; i++)
    {
        if(string_chars[i]==c) return 1;
    }
    return 0;
}

char* quitar_caracteres(char* string, char* c)
{
    int i;
    int j;
    for(i=0 ; i < strlen(string) ; i++)
    {
        if(hayQueEliminar(string[i], c))
        {
            for(j=i ; j<strlen(string)-1 ;j++)
            {
                string[j] = string[j+1];
            }
        }
        string[j]='\0';
        i--;
    }

    return string;
}

