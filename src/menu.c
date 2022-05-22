#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "book.h"

#include "heap.h"

void showMenu() {
    printf("Biblioteca\n");
    printf("1: Importar archivos\n");
    printf("2: Mostrar libros\n");
    printf("3: Buscar libro por coincidencias\n");
    printf("4: Palabras con mayor frecuencia\n");
    printf("5: Palabras más relevantes\n");
    printf("6: Buscar libros por palabra\n");
    printf("7: Mostrar palabra en contexto\n");
    printf("0: Salir\n");
}

//Función que muestra las palabras con mayor frecuencia
void mostFrequency(OrderedTreeMap* sortedBooks)
{
    char in[30];
    printf("Ingrese el ID del libro\n");
    scanf("%s", in);
    getchar();
    Pair *bookPair = firstOrderedTreeMap(sortedBooks);
    Book *book;
    if (bookPair == NULL)
    {
        printf("No se ha ingresado ningun documento\n");
        return;
    }

    while (bookPair != NULL)
    {
        book = bookPair->value;

        //Recorre todos los libros existentes, si existe muestra sus datos y termina la función.
        if (strcmp(in, book->id) == 0)
        {
            //Se muestran los datos del libro
            printf("ID: %s\n", book->id);
            printf("Title: %s\n", book->title);
            printf("Populares: \n");
            HashMapPair *aux = firstMap(book->wordFrequency);
            Mheap *heap = createMheap();

            //Se añaden todas las palabras el Heap para tenerlas ordenadas
            while (aux != NULL) 
            {
                heap_push(heap, aux->value, ((Word*)(aux->value))->frequency );
                aux = nextMap(book->wordFrequency);
            }
            //Se muestran las 10 palabras con mayor frecuencia del texto.
            for (int i = 0; i < 10; i++) 
            {
                Word* word = heap_top(heap);
                heap_pop(heap);
                printf("%s: %lf\n", word->word, word->frequency);
            }
            return;
        }
        bookPair = nextOrderedTreeMap(sortedBooks);
    }
    //Caso en donde no se encuentra el ID ingresado.
    printf("El documento que ingreso no existe\n");
}

/* Funcion que muestra los libros que contienen 
 * todas las palabras ingresadas*/
void bookWithWords(OrderedTreeMap* sortedBooks)
{
    char in[100];
    printf("Ingrese las palabras a buscar: ");
    scanf("%[^\n]s", in);
    getchar();
    stringToLower(in);//Se pasan todas las palabras a minusculas.
    List *ret = strToList(in, " ");//Se seaparan por el espacio y se crea una lista con cada palabra.

    Pair *bookPair = firstOrderedTreeMap(sortedBooks);
    //En caso de que ningun documento a sido leido
    if (bookPair == NULL)
    {
        printf("No se ha ingresado ningun documento\n");
        return;
    }
    Book *book;
    int flag;//Si es 0 una palabra no se encontraba/si es 1 se encontraba.
    HashMapPair *auxWrd;
    //Se recorren todos los libros.
    while (bookPair != NULL)
    {
        flag = 1;
        book = bookPair->value;
        char* words = (char*)listFirst(ret);
        //Se recorre la lista completa de palabras ingresadas
        while (words != NULL)
        {
            auxWrd = searchMap(book->wordFrequency, words);
            //Si la palabra no se encontraba se cambia el valor a 0
            if (auxWrd == NULL) 
            {
                flag = 0;
            }
            words = (char*)listNext(ret);
        }
        //Si todas las palabras se encontraban se muestra el titulo.
        if (flag == 1)
        {
            printf("%s\n", book->title);
        }
        bookPair = nextOrderedTreeMap(sortedBooks);
    }
}

void 
showBooks(OrderedTreeMap *sortedMap) 
{
    Pair *bookPair = firstOrderedTreeMap(sortedMap);
    Book *book;
    if (bookPair == NULL)
    {
        printf("No se ha ingresado ningun documento\n");
        return;
    }

    //Ciclo que recorre todos los libros ingresados.
    while (bookPair != NULL) 
    {
        book = bookPair->value;

        printf("ID: %s\n", book->id);
        printf("Title: %s\n", book->title);
        printf("Cantidad de palabras: %ld\n", book->wordCount);
        printf("Cantiad de caracteres: %ld\n", book->charCount);
        bookPair = nextOrderedTreeMap(sortedMap);
    }
}

void 
searchBooks(OrderedTreeMap *map, int docCount, HashMapSus *fileAppeareances) 
{
    char in[100];
    printf("Ingrese la palabra a buscar: ");
    scanf("%[^\n]*s", in);
    getchar();
    Pair *aux = firstOrderedTreeMap(map);
    Book *auxBook = NULL;
    Mheap *heap = createMheap();
    //Se recorren los libros
    while (aux != NULL)
    {
        auxBook = aux->value;
        HashMapPair * auxWord = searchMap(auxBook->wordFrequency, in);
        //Se recorren las palabras
        if (auxWord != NULL) 
        {
            Word * auxWordWord = auxWord->value;
            
            HashMapSusPair *auxint = searchMapSus(fileAppeareances, in);
            //Si el dato obtenido no es NULL se obtiene la frecuencia y relevancia.
            if (auxint) {
                auxWordWord->frequency = frequency(auxWordWord->appearances, auxBook->wordCount);
                setWordRelevance(auxWordWord, docCount, auxint->value);
            }
            heap_push(heap, auxBook, auxWordWord->relevance);
        }
        aux = nextOrderedTreeMap(map);
    }

    if (heap_top(heap) == NULL)
    {
        printf("Ningun libro contenia la palabra\n");
    }

    //Muestra los libros ordenados por relevancia.
    while (heap_top(heap)) 
    {
        auxBook = heap_top(heap);
        HashMapPair* auxWord = searchMap(auxBook->wordFrequency, in);
        showBook(auxBook, auxWord->value);
        heap_pop(heap);
    }
}

void showList (List* list)
{
    char* id = listFirst(list);

    while (id != NULL)
    {
        printf("%s\n", id);
        id = listNext(list);
    }     
}

List * readBooks() {
    printf("Ingrese los ID de los libros a leer: \n");
    char ids[512];

    fflush(stdin);
    scanf("%[^\n]*s", ids);
    getchar();

    List *ret = strToList(ids, " ");

    return ret;
}
