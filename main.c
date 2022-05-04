#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

//Crea la lista de generos y guarda los mismos al detectarlos
List *strToList(const char *genre, char delim){
    List *list = listCreate();

    char *aux = _strdup(genre);
    char *token = strtok(aux, delim);

    int flag = 0;
    while(token != NULL){
        if (flag) listPushBack(list, token+1);
        else listPushBack(list, token);
        flag = 1;

        token = strtok(NULL, delim);
    }
        
    return list;
}

List * readBooks() {
    printf("Ingrese los ID de los libros a leer: ");
    char in[100];

    fflush(stdin);
    scanf("%[^\n]*s", in);
    getchar();

    List *ret = listCreate();

}

void lower_than(void *k1, void *k2);
int main() {
    List *books = readBooks();

    return 0;
}
