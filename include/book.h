#ifndef BOOK_H
#define BOOK_H
#include "list.h"
#include <stdio.h>
#include "word.h"
#include "util.h"

#include "treemap.h"
#include "hashmap.h"


typedef struct Book {
    // Mapa de palabras
    HashMap* wordFrequency;
    char title[100];
    char id[30];
    FILE* fd;
    long charCount;
    long wordCount;
} Book;

void showList (List* list);

List * readBooks();

Book *createBook(char *id, HashMapSus* fileAppearances);

void loadBooks(List *books, OrderedTreeMap *sortedBooks, 
        HashMapSus* fileAppearances, int *count);
void showBooks(OrderedTreeMap *sortedMap);

void countWords(Book *book, HashMapSus* fileAppearances);

void searchBooks(OrderedTreeMap *map, int docCount, HashMapSus *fileAppearances);

void showBook(Book *book, Word *word);

void getRelevance (OrderedTreeMap *map, int totalDocuments, HashMap* fileAppearances);

void relevantWords(OrderedTreeMap* sortedBooks, HashMapSus *fileAppeareances, long docCount);

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
