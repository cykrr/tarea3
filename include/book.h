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
 *  @public @memberof Menu
 */
Book *createBook(char *id);


/** @brief Carga los libros y los ordena en el `OrderedTreeMap` `sortedBooks`
 *
 *  @param books Lista con los ID como string de los libros a cargar
 *  
 *  @param sortedBooks `OrderedTreeMap` donde se ordenarán los libros 
 *      una vez cargados.
 *
 *  @param fileAppearances HashMap con la cantidad de ocurrencias de
 *      todas las palabras en todos los documentos.
 *
 *  @param count Puntero a `int` con la cantidad de documentos cargados.
 *
 *  Procesa la lista de strings `books`, la cual posee los ID de los
 *  libros a importar. Por cada string encontrada en la lista 
 *  se crea un tipo `Book`.
 *  Si el `Book` creado no se encuentra en el `OrderedTreeMap` `sortedBooks`
 *  entonces se contarán las palabras y se calculará la frecuencia de 
 *  las palabras en dicho libro. Si todo sale bien se insertará el libro
 *  en el `OrderedTreeMap` `sortedBooks` y se aumenta el contador de libros
 *  `count`.
 *  
 *  @public @memberof Book
 */
void loadBooks(List *books, OrderedTreeMap *sortedBooks, 
        HashMapSus* fileAppearances, int *count);

/** @brief muestra todos los libros en el Mapa `sortedMap`
 *  
 *  @param sortedMap `OrderedTreeMap` con todos los libros ordenados por título.
 *
 * Muestra el ID, Titulo, Cantidad de palabras y Cantidad de caracteres de todos los 
 * libros presentes.
 *
 *  @public @memberof Menu
 */
void showBooks(OrderedTreeMap *sortedMap);

/** @brief Cuenta la cantidad de apariciones de las palabras.
 *  
 *  @param book Libro a procesar
 *
 *  @param fileAppearances HashMap con todas las apariciones 
 *      de todas las palabras en todos los documentos.
 *
 *  Recorre el libro palabra por palabra y elimina los caracteres
 *  que intervengan con el análisis. Se convierte temporalmente
 *  la palabra a minusculas y se revisa si está en los mapas.
 *  De ser así, se le aumenta uno a su contador y de lo
 *  contrario se añade.
 *
 *  @public @memberof Menu
 */
void countWords(Book *book, HashMapSus* fileAppearances);

/**
 *  @brief Función del menú que busca un libro en base a la coincidencia
 *      de palabras
 *
 *  @param map Mapa Ordenado con los libros ordenados por titulo.
 *
 *  @param docCount Cantidad de documentos cargados.
 *
 *  @param fileAppearances HashMap con todas las apariciones 
 *      de todas las palabras en todos los documentos.
 *
 *  Esta función recorre todos los libros buscando la palabra seleccionada
 *  Si el libro contiene la palabra que ingresó el usuario se inserta en 
 *  un montículo para así mostrarlas ordenadas por su frecuencia.
 * @public @memberof Menu
 */
void searchBooks(OrderedTreeMap *map, int docCount, HashMapSus *fileAppearances);

/** 
 * @brief Muestra el ID, Titulo, Apariciones y relevancia de un libro
 * @param book libro a mostrar.
 * @param word palabra de la cual mostrar detalles.
 */
void showBook(Book *book, Word *word);

/** @brief Obtiene la relevancia de todas las palabras
 *
 *  @param map Mapa con todos los libros ordenados por nombre
 *
 *  @param totalDocuments cantidad total de documentos cargados
 *
 *  @param fileAppearances HashMap con todas las apariciones 
 *      de todas las palabras en todos los documentos.
 *
 *  Recorre el mapa de libros y libro por libro calcula
 *  la relevancia de todas las palabras.
 *  @public @memberof Book
 */ 

void relevantWords(OrderedTreeMap* sortedBooks, HashMapSus *fileAppeareances, long docCount);


/** @brief Muestra las palabras con mayor frecuencia
 *
 *  @param sortedBooks Mapa de todos los libros ordenados por nombre.
 *
 *  Dado un ID de un libro específico muestra sus palabras con mayor 
 *  frecuencia.
 *  Si no se han ingresado libros. Se avisa y se termina la función
 *  De lo contrario se recorren los libros y se detiene al encontrar
 *  una coincidencia en el ID, posteriomente muestra la información
 *  del libro ID, Titulo y populares, luego imprime las
 *  palabras ordenadas por su frecuencia.
 *
 *  @public @memberof Menu
 */
void mostFrequency(OrderedTreeMap* sortedBooks);


/** @brief Buscar un libro por coincidencias 
 *  @param sortedBook Mapa ordenado con todos los libros ordenados 
 *      por titulo
 *
 *   Recibe las palabras a buscar separadas por un espacio.
 *   Se separan estas palabras a una lista enlazada para
 *   posteriormente recorrerla y mostrar el titulo de los
 *   libros si estos poseen alguna de las palabras en 
 *   cuestión.
 *
 *  @public @memberof Menu
 */
void bookWithWords(OrderedTreeMap* sortedBooks);

/** @brief Llenar el mapa con las palabras correspondientes.
 *
 *  Crea un Map con las palabras a excluir para
 *  reducir la complejidad de su obtención.
 *  @public @memberof Menu
 *  @return Mapa Ordenado con las palabras a excluir.
 */
OrderedTreeMap *populateExcludeMap();

/** @brief Calcula la frecuencia relevancia función de sus parámetros
 *  @param documentCount La cantidad de documentos. Se actualiza
 *      cada vez que se añade un libro.
 *
 *  @param totalMatches Coincidencias en todos los libros.
 *      Se actualiza cada vez que se añade un libro.
 *  @param frequency Frecuencia de la palabra en el libro.
 *
 *  @private @memberof Word
 */
double relevance(
                float documentCount,
                float totalMatches, 
                float frequency);

/**  @brief Calcula la frecuencia de la palabra en el libro.
 *  @param bookMatches coincidencias en el libro. Se mantiene constante. 
 *  @param wordsInBook palabras por libro. Se mantiene constante. 
 *  @private @memberof Word
 */
double frequency( 
                double bookMatches,
                double wordsInBook
);

/** @brief Establece la frecuencia de todas las palabras en el libro 
 *  @param book Libro del cual calcular la frecuencia
 */
void setBookFrequency(Book* book);
/** @brief Establece la relevancia de una palabra determinada 
 *  @param word Palabra a modificar.
 *  @param docCount Cantidad de documentos cargados
 *  @param totalMatches Cantidad de coincidencias de la palabra
 *      en todos los documentos.
 *
 *  Establece la relevancia de la palabra basándose en su frecuencia
 *  previamente calculada pues no es necesario calcularla de nuevo.
 *  @public @memberof Word
 */
void setWordRelevance(Word *word, double docCount, double totalMatches);

#endif
