#ifndef BOOK_H
#define BOOK_H
#include "treemap.h"
#include "list.h"
#include <stdio.h>
#include "word.h"
#include "util.h"


typedef struct Book {
    // Mapa de palabras
    OrderedTreeMap* wordFrequency;
    char title[100];
    char id[30];
    FILE* fd;
    long charCount;
    long wordCount;
} Book;

void showList (List* list);

List * readBooks();

Book *createBook(char *id, OrderedTreeMap* fileAppearances);

void loadBooks(List *books, OrderedTreeMap *sortedBooks, 
        OrderedTreeMap* fileAppearances, int *count);
void showBooks(OrderedTreeMap *sortedMap);

void countWords(Book *book, OrderedTreeMap* fileAppearances);

void searchBooks(OrderedTreeMap *map, int docCount, OrderedTreeMap *fileAppearances);

void showBook(Book *book, Word *word);

void getRelevance (OrderedTreeMap *map, int totalDocuments, OrderedTreeMap* fileAppearances);

void relevantWords(OrderedTreeMap* sortedBooks, OrderedTreeMap *fileAppeareances, long docCount);

void mostFrequency(OrderedTreeMap* sortedBooks);

void getFrequency(OrderedTreeMap* sortedBooks);

void bookWithWords(OrderedTreeMap* sortedBooks);

// Llenar el mapa con las palabras correspondientes.
//

OrderedTreeMap *populateExcludeMap();

double relevance(
        /* la cantidad de documentos. Se actualiza cada vez que
         * se añade un libro. */
                float documentCount,
        /*  coincidencias en todos los libros. Se actualiza cada vez
         *  que se añade un libro. */
                float totalMatches, 
        /*  Frecuencia de la palabra en el libro */
                float frequency);

double 
frequency( 
        /*  coincidencias en el libro. Se mantiene constante. */
                double bookMatches,
        /*  palabras por libro. Se mantiene constante. */
                double wordsInBook
);


void setBookFrequency(Book* book);
void setWordRelevance(Word *word, double docCount, double totalMatches);
#endif
