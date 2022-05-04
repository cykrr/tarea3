#ifndef BOOK_H
#define BOOK_H
#include "treemap.h"
#include "list.h"
#include <stdio.h>

typedef struct Book {
    // Mapa de palabras
    TreeMap* wordFrequency;
    char title[100];
    char id[30];
    FILE* fd;
} Book;

void showList (List* list);

List * readBooks();

#endif
