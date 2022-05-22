#ifndef WORD_H
#define WORD_H
#include "list.h"
#include "treemap.h"

/** @brief Almacena información de las palabras
 */
typedef struct Word { 
    char word[100]; /**< Almacena el string de la palabra.*/
    double frequency; /**< Almacena la frecuencia de la palabra en el documento. */
    int appearances; /**< Almacena las apariciones de la palabra en el documento. */
    double relevance; /**< Almacena la relevancia de la palabra. */
} Word;

/** @brief Muestra Una palabra en su contexto.
 *  @public @memberof Menu
 */
void showWordContext(OrderedTreeMap *map);


/** @brief Llenar el mapa con las palabras correspondientes.
 *
 *  Crea un Map con las palabras a excluir para
 *  reducir la complejidad de su obtención.
 *  @public @memberof Word
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

/** @brief Llenar el mapa con las palabras correspondientes.
 *
 *  Crea un Map con las palabras a excluir para
 *  reducir la complejidad de su obtención.
 *  @public @memberof Word
 *  @return Mapa Ordenado con las palabras a excluir.
 */
OrderedTreeMap *populateExcludeMap();

#endif
