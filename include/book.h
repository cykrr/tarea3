#ifndef BOOK_H
#define BOOK_H
#include "treemap.h"
#include "list.h"
#include <stdio.h>
#include "word.h"
#include "util.h"


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

void relevantWords(TreeMap* sortedBooks, TreeMap *fileAppeareances, long docCount);

void mostFrequency(TreeMap* sortedBooks);

void getFrequency(TreeMap* sortedBooks);

void bookWithWords(TreeMap* sortedBooks);

// Llenar el mapa con las palabras correspondientes.
//

TreeMap *populateExcludeMap();

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
