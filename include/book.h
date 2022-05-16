#ifndef BOOK_H
#define BOOK_H
#include "treemap.h"
#include "list.h"
#include <stdio.h>
#include "word.h"

typedef struct Book {
    // Mapa de palabras
    TreeMap* wordFrequency;
    char title[100];
    char id[30];
    FILE* fd;
    long charCount;
    long wordCount;
} Book;

void showList (List* list);

List * readBooks();

Book *createBook(char *id);

void loadBooks(List *books, TreeMap *sortedBooks);
void showBooks(TreeMap *sortedMap);

void countWords(Book *book);

void searchBooks(TreeMap *map);

void showBook(Book *book, Word *word);

#endif
