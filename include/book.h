#ifndef BOOK_H
#define BOOK_H
#include "list.h"
#include <stdio.h>
#include "word.h"
#include "util.h"

#include "treemap.h"
#include "hashmap.h"


/** @brief Almacena información del libro
 */ 
typedef struct Book {
    // Mapa de palabras
    HashMap* wordFrequency; /**< Mapa que almacena la frecuencia de las palabras.*/
    char title[100]; /**< Almacena el título del libro. */
    char id[30]; /**< Almacena el ID del libro*/
    FILE* fd; /**< Almacena el file-descriptor del libro. */
    long charCount; /**< Almacena la cantidad de carácteres en el libro. */
    long wordCount; /**< Almacena la cantidad de palabras en el libro. */
} Book;

/** @brief Muestra una lista de libros
 *  @param list Lista del tipo Libro que contiene los libros a mostrar
 *  @public @memberof Book
 */
void showList (List* list);


/** @brief Recibe la entrada de los libros a leer
 *  @return Lista enlazada con un `Book` por cada ID, en caso de que el 
 *      archivo exista.
 *  @public @memberof Book
 */
List * readBooks();

/** @brief Constructor de `Book`
 *
 *  @param id ID del libro a procesar.
 *
 *  @param fileAppearances Mapa con la cantidad de ocurrencias de todas 
 *      las palabras en todos los documentos.
 *
 *  Esta función crea un `Book` en función del ID del
 *  documento a cargar. Inicializa la estructura interna 
 *  del Libro, tal como crear el mapa de frecuencia, 
 *  guardar el ID, obtener el nombre del archivo y verificar
 *  que este existe.
 *
 *  @return `Book` inicializado.
 *  @public @memberof Book
 */
Book *createBook(char *id);


/** @brief Carga los libros y los ordena en el `OrderedTreeMap` `sortedBooks`
 *
 *  Procesa la lista de strings `books`, la cual posee los ID de los
 *  libros a importar. Por cada string encontrada en la lista 
 *  se crea un tipo `Book`.
 *  
 *  @public @memberof Book
 */
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
