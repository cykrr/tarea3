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

Book *createBook(char *id, TreeMap* fileAppearances);

void loadBooks(List *books, TreeMap *sortedBooks, 
        TreeMap* fileAppearances, int *count);
void showBooks(TreeMap *sortedMap);

void countWords(Book *book, TreeMap* fileAppearances);

void searchBooks(TreeMap *map);

void showBook(Book *book, Word *word);

void getRelevance (TreeMap *map, int totalDocuments, TreeMap* fileAppearances);

void relevantWords(TreeMap* sortedBooks);

void mostFrequency(TreeMap* sortedBooks);

void getFrequency(TreeMap* sortedBooks);

void bookWithWords(TreeMap* sortedBooks);

#endif
