#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>
#include "util.h"

void showList (List* list)
{
    char* id = listFirst(list);

    while (id != NULL)
    {
        printf("%s\n", id);
        id = listNext(list);
    }     
}

List * readBooks() {
    printf("Ingrese los ID de los libros a leer: ");
    char ids[100];

    fflush(stdin);
    scanf("%[^\n]*s", ids);
    getchar();

    List *ret = strToList(ids, " ");

    return ret;
}

void lower_than(void *k1, void *k2);
int main() {
    List *books = readBooks();
    showList (books);

    return 0;
}
