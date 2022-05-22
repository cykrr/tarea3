#ifndef MENU_H
#define MENU_H
#include "treemap.h"
#include "hashmap.h"
#include "list.h"

/** @brief Muestra el menú principal del programa
 */
void showMenu();

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

/** @brief Muestra una lista de libros
 *  @param list Lista del tipo Libro que contiene los libros a mostrar
 *  @public @memberof Menu
 */
void showList (List* list);


/** @brief Recibe la entrada de los libros a leer
 *  @return Lista enlazada con un `Book` por cada ID, en caso de que el 
 *      archivo exista.
 *  @public @memberof Menu
 */
List * readBooks();

#endif
