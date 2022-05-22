#include "book.h"
#include "list.h"
#include "util.h"
#include "word.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"


/* Es necesario actualizar en cada palabra del libro 
 * la cantidad de coincidencias, así como actualizarlas 
 * en las coincidencias en todos los libros. Esto se 
 * realiza en la función countWords()*/

Book*
createBook(char *id)
{
    Book *book = malloc(sizeof(Book));
    book->charCount = 0;
    book->wordCount = 0;
    if(!book) 
    {
        printf("Error guardando memoria para Book\n");
    }

    book->wordFrequency = createMap(100);
    strcpy(book->id, id);

    char fileName[100];
    strcpy(fileName, "books/");
    strcat(fileName, id);
    strcat(fileName, ".txt");

    book->fd = fopen(fileName, "r");

    if(!book->fd) {
        printf("El archivo %s no existe\n", fileName);
        free(book);
        return NULL; 
    }

    char word[100] = "";
    //Se lee la primera linea hasta encontrar un of.
    while (strcmp(word, "of") != 0)
        fscanf(book->fd, "%99s", word);

    //Se quita el espacio que viene despues del of.
    fgetc(book->fd);
    //Se obtiene el resto de la linea.
    fgets(book->title, 99, book->fd);

    // Eliminar el Autor si es que existe.
    char *comma = strchr(book->title, ',');
    if(comma != NULL) *comma = '\0';

    // Eliminar salto de linea
    char *newline = strchr(book->title, '\n');
    if(newline != NULL) *newline = '\0';

    // Eliminar salto de linea
    char *cr = strchr(book->title, '\r');
    if(cr != NULL) *cr = '\0';


    return book;
}


void loadBooks(List* books, OrderedTreeMap* sortedBooks, HashMapSus* fileAppearances, int *count)
{
    char* id = listFirst(books);
    while (id != NULL)
    {
        Book *book = createBook(id);

        //Si el libro no se encontraba se agrega y se cuentan las palabras y se calcula su frecuencia.
        if(book != NULL && searchOrderedTreeMap(sortedBooks, book->title) == NULL)  {
            countWords(book, fileAppearances);
            setBookFrequency(book);
            insertOrderedTreeMap(sortedBooks, book->title, book);
            *count += 1;

        }

        id = listNext(books);
    }
}

/* contamos una vez por cada libro su cantidad de palabras y
 * caracteres. A la vez las añadimos al mapa de ocurrencias 
 * en todos los libros (fileAppeareances). También obtenemos
 * la posición de la palabra */
void 
countWords(Book *book, HashMapSus* fileAppearances)
{
    //Mapa que contiene las palabras a excluir.
    OrderedTreeMap *exclude = populateExcludeMap();

    char * x = malloc(1024 * sizeof (char) );
    rewind(book->fd);//Se devuelve al comienzo del archivo

    // Se obtiene el archivo palabra por palabra.
    while (fscanf(book->fd, " %1023s", x) == 1) 
    {
        book->charCount += strlen(x);
        (book->wordCount)++;
        quitar_caracteres(x, "[]{}’”“?,.\":;/!-_()\'=*%%");
        stringToLower(x);

        //Si la palabra se tiene que excluir no se cuenta.
        if (searchOrderedTreeMap(exclude, x) != NULL)
        {
            continue;
        }

        HashMapPair *aux = searchMap(book->wordFrequency, x);

        //Si la pabra no se encontraba se inserta en el mapa
        if (aux == NULL)
        {
            Word *word = malloc(sizeof(Word));
            strcpy(word->word, x);
            word->appearances = 1;//Apariciones se inicializa en 1

            insertMap(book->wordFrequency, word->word, word);
            //Contar apariciones de una palabra en el archivo
            HashMapSusPair *tmp = searchMapSus(fileAppearances, word->word);

            //Si la palabra ya se encontraba en el mapa de apariciones se aumenta
            if (tmp != NULL) 
            {
                (tmp->value)++;
            }
            //Sino se inserta y se inicializa en 1. 
            else 
            {
                insertMapSus(fileAppearances, word->word, 1);
            }
        } 
        //Si existe solo aumentamos las apariciones.
        else 
        {
            ((Word*)(aux->value))->appearances++;
        }
   }
}


void 
showBook(Book *book, Word *word) 
{
    printf("ID: %s\n", book->id);
    printf("Titulo: %s\n", book->title);
    printf("Apariciones: %d\n", word->appearances);
    printf("Relevancia: %lf\n", word->relevance);
}


void relevantWords(OrderedTreeMap* sortedBooks, HashMapSus *fileAppeareances, long docCount)
{
    char in[100];
    printf("Ingrese el libro a buscar: ");
    scanf("%[^\n]*s", in);
    getchar();

    /* Vemos si el libro existe */
    Pair *aux = searchOrderedTreeMap(sortedBooks, in);
    if (aux == NULL)
    {
        printf("El libro que ingreso no existe\n");
        return;
    } else {
        printf("Palabras más relevantes de \'%s\':\n", (char*)aux->key);
    }
    Book *auxBook = aux->value;
    Mheap *heap = createMheap();



    HashMapPair* auxWord = firstMap(auxBook->wordFrequency);
    //Se recorre el mapa de palabras.
    while (auxWord != NULL) 
    {
        Word * auxWordWord = auxWord->value;
        HashMapSusPair *aux = searchMapSus(fileAppeareances, auxWordWord->word);
        if(aux != NULL)
            setWordRelevance(auxWordWord, docCount, aux->value);
        /*else 
            setWordRelevance(auxWordWord, docCount, 1);*/
        //Se inserta la palabra en el heap con relevancia como prioridad.
        heap_push(heap, auxWordWord, auxWordWord->relevance);
        auxWord = nextMap(auxBook->wordFrequency);
    }

    int i = 0;
    //Se muestran solo las 10 palabras mas relevantes del libro.
    while (i < 10)
    {
        Word *word = heap_top(heap);
        printf("%s: %lf\n", word->word, word->relevance);
        heap_pop(heap);
        i++;
    }
    printf("!%ld\n", docCount);
}



void setWordFrequency(Word* word, long wordsInBook)
{
    word->frequency = frequency(word->appearances, wordsInBook);
}

//Se obtiene la frecuencia de todas las palabras. Se ejecuta 
// cuando se añade el libro.
void setBookFrequency(Book* book)
{
    HashMapPair *aux = firstMap(book->wordFrequency);
    while (aux != NULL)
    {
        Word *auxWord = aux->value;
        setWordFrequency(auxWord, book->wordCount);
        aux = nextMap(book->wordFrequency);
    }
}



void updateBookRelevance(Book *book) {

}


