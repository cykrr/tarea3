#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>
#include "util.h"


List * readBooks() {
    printf("Ingrese los ID de los libros a leer: ");
    char ids[100];

    fflush(stdin);
    scanf("%[^\n]*s", ids);
    getchar();

    List *ret = strToList(ids, ' ');

    return ret;
}

void lower_than(void *k1, void *k2);
int main() {
    List *books = readBooks();

    return 0;
}
