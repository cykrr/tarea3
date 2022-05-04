#ifndef BOOK_H
#define BOOK_H
#include "treemap.h"

typedef struct Book {
    // Mapa de palabras
    TreeMap* wordFrequency;
    char title[60];
    char id[30];
} Book;

void showList (List* list);

List * readBooks();

#endif