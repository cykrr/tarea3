#include "book.h"
#include "list.h"
#include "util.h"
#include "word.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "hashmap.h"


int inverse_lower_than_string(void* key1, void* key2){
    char* k1=(char*) key1;
    char* k2=(char*) key2;
    if(strcmp(k2,k1)<0) return 1;
    return 0;
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
    printf("Ingrese los ID de los libros a leer: ");
    char ids[100];

    fflush(stdin);
    scanf("%[^\n]*s", ids);
    getchar();

    List *ret = strToList(ids, " ");

    return ret;
}

/* Es necesario actualizar en cada palabra del libro 
 * la cantidad de coincidencias, así como actualizarlas 
 * en las coincidencias en todos los libros. Esto se 
 * realiza en la función countWords()*/

Book*
createBook(char *id, HashMapSus* fileAppearances)
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
    while (strcmp(word, "of") != 0)
        fscanf(book->fd, "%99s", word);

    fgetc(book->fd);
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

void showBooks(OrderedTreeMap *sortedMap) 
{
    Pair *bookPair = firstOrderedTreeMap(sortedMap);
    Book *book;
    if (bookPair == NULL)
    {
        printf("No se ha ingresado ningun documento\n");
        return;
    }

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

void loadBooks(List* books, OrderedTreeMap* sortedBooks, HashMapSus* fileAppearances, int *count)
{
    char* id = listFirst(books);
    while (id != NULL)
    {
        Book *book = createBook(id, fileAppearances);

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
    OrderedTreeMap *exclude = populateExcludeMap();

    /*
    printf("INICIO MAPA\n");
    Pair *aux = firstOrderedTreeMap(exclude);
    while (aux != NULL) {
        char *i = aux->value;
        printf("%s\n", i);
        aux = nextOrderedTreeMap(exclude);
    }
    printf("FINAL MAPA\n");
    */

    char * x = malloc(1024 * sizeof (char) );
    rewind(book->fd);
    // palabra x palabra
    while (fscanf(book->fd, " %1023s", x) == 1) 
    {
        book->charCount += strlen(x);
        (book->wordCount)++;
        quitar_caracteres(x, "[]{}’”“?,.\":;/!-_()\'=*%%");
        stringToLower(x);
     //   printf("%s: ",x );

        if (searchOrderedTreeMap(exclude, x) != NULL)
        {
            continue;
        }

      //  printf("\n");

        //printf("%s ", (char*)x);
        HashMapPair *aux = searchMap(book->wordFrequency, x);

        if (aux == NULL)
        {
            Word *word = malloc(sizeof(Word));
            strcpy(word->word, x);
            word->appearances = 1;

            insertMap(book->wordFrequency, word->word, word);
            //Contar apariciones de una palabra en el archivo
            HashMapSusPair *tmp = searchMapSus(fileAppearances, word->word);
            if (tmp != NULL) 
            {
                (tmp->value)++;
            } else {
                insertMapSus(fileAppearances, word->word, 1);
            }
        } 
        else 
        {
            ((Word*)(aux->value))->appearances++;
        }
   }
}

void 
searchBooks(OrderedTreeMap *map, int docCount, HashMapSus *fileAppeareances) 
{
    char in[50];
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
            printf("gol\n");
            if (auxint) {
                auxWordWord->frequency = frequency(auxWordWord->appearances, auxBook->wordCount);
                setWordRelevance(auxWordWord, docCount, auxint->value);
                printf("raios%d %d %d\n", auxWordWord->appearances, docCount, auxint->value);

            }
            heap_push(heap, auxBook, auxWordWord->relevance);
        }
        aux = nextOrderedTreeMap(map);
    }

    if (heap_top(heap) == NULL)
    {
        printf("Ningun libro contenia la palabra\n");
    }

    while (heap_top(heap)) 
    {
        auxBook = heap_top(heap);
        HashMapPair* auxWord = searchMap(auxBook->wordFrequency, in);
        showBook(auxBook, auxWord->value);
        heap_pop(heap);
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

void getRelevance (OrderedTreeMap *map, int totalDocuments, HashMap* fileAppearances)
{
    Pair *aux = firstOrderedTreeMap(map);
    while (aux != NULL) {
//        printf("Libro: %s\n", ((Book*)aux->value)->title);
        HashMapPair *aux2 = firstMap(((Book*)aux->value)->wordFrequency);
        while (aux2 != NULL) {
//            printf("Palabra: %s ", (char*)aux2->key);

            
            HashMapPair *tmp = searchMap(fileAppearances, aux2->key);

            double a = (double)(((Word*)(aux2->value))->appearances) / (double)((Book*)(aux->value))->wordCount;
                 a *= 
                     logf(
                         ((double)totalDocuments)
                       / 
                         (double)(*(int*)(tmp->value))
                  );
            ((Word*)(aux2->value))->relevance = a;
     //       printf("%d\n", totalDocuments);
     //       printf("%d\n", ((Word*)(aux2->value))->appearances);
     //       printf("%ld\n", ((Book*)(aux->value))->wordCount);
     //       printf("%d\n", *((int*)(tmp->value)));
            printf("%lf\n", a);

            aux2 = nextMap(((Book*)aux->value)->wordFrequency);
        }

        aux = nextOrderedTreeMap(map);
    }
}

void relevantWords(OrderedTreeMap* sortedBooks, HashMapSus *fileAppeareances, long docCount)
{
    char in[100];
    printf("Ingrese el libro a buscar: ");
    scanf("%[^\n]*s", in);
    getchar();


    /*
    Pair *a = firstOrderedTreeMap(sortedBooks);
    int j = 0;
    while (a != NULL) {
        char *i = a->key;
        printf("%d\n %s..\n",j, i);
        j++;

        a = nextOrderedTreeMap(sortedBooks);
    }
   
    */

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
    while (auxWord != NULL) 
    {
        Word * auxWordWord = auxWord->value;
        HashMapSusPair *aux = searchMapSus(fileAppeareances, auxWordWord->word);
        if(aux != NULL)
        setWordRelevance(auxWordWord, docCount, aux->value);
        else 
            setWordRelevance(auxWordWord, docCount, 0);
        heap_push(heap, auxWordWord, auxWordWord->relevance);
        auxWord = nextMap(auxBook->wordFrequency);
    }

    int i = 0;
    while (i < 10 )
    {
        Word *word = heap_top(heap);
        printf("%s: %lf\n", word->word, word->relevance);
        heap_pop(heap);
        i++;
    }
    printf("!%ld\n", docCount);
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

void setWordRelevance(Word *word, double documentCount, double totalMatches) {
    word->relevance = relevance(documentCount, totalMatches, word->frequency);
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
void bookWithWords(OrderedTreeMap* sortedBooks)
{
    char in[100];
    printf("Ingrese las palabras a buscar: ");
    scanf("%[^\n]s", in);
    getchar();
    stringToLower(in);
    List *ret = strToList(in, " ");

    Pair *bookPair = firstOrderedTreeMap(sortedBooks);
    if (bookPair == NULL)
    {
        printf("No se ha ingresado ningun documento\n");
        return;
    }
    Book *book;
    int flag;
    HashMapPair *auxWrd;
    while (bookPair != NULL)
    {
        flag = 1;
        book = bookPair->value;
        char* words = (char*)listFirst(ret);
        while (words != NULL)
        {
            auxWrd = searchMap(book->wordFrequency, words);
            if (auxWrd == NULL) flag = 0;
            words = (char*)listNext(ret);
        }
        if (flag == 1)
        {
            printf("%s\n", book->title);
        }
        bookPair = nextOrderedTreeMap(sortedBooks);
    }
}

OrderedTreeMap *
populateExcludeMap() 
{
    OrderedTreeMap *excludeMap = createOrderedTreeMap(lower_than_string);

    char excludeWords[][100] = {"the", "and", "of", "to", "that", 
        "in", "he", "shall", "unto", "for", "a", "was", "it",
        "she", "said", "you", "be", "an", "have", "i", "not",
        "on", "with", "as", "do", "at", "this", "but", "his",
        "by", "from", "they", "we", "say", "her", "or", "will",
        "my", "one", "all", "would", "there", "their", "what", 
        "so", "up", "out", "if", "about", "who", "get", "which",
        "go", "me", "when", "make", "can", "like", "no", "just",
        "him", "know", "take", "into", "your", "good", "some", 
        "could", "them", "than", "then", "now", "come", "its",
        "also", "how", "our", "well", "even", "want", "because",
        "any", "most", "us", "are", "is", "had", "were", "went", 
        "ye", "thee", "thou", "thy", "hath", "has", "may", "more"};
    int count = sizeof(excludeWords) / (100 * sizeof(char));
    for (int i = 0; i < count; i++) {
        char *dup = _strdup(excludeWords[i]);
        insertOrderedTreeMap(excludeMap, dup, dup);
    }
    return excludeMap;
}

void updateBookRelevance(Book *book) {

}


double 
relevance
(
        /* la cantidad de documentos. Se actualiza cada vez que
         * se añade un libro. */
                float documentCount,
        /*  coincidencias en todos los libros. Se actualiza cada vez
         *  que se añade un libro. */
                float totalMatches, 
        /* frecuencia de la palabra en el libro */
                float frequency
)   {
        
        return frequency *= logf(documentCount/totalMatches);
}

double 
frequency( 
        /*  coincidencias en el libro. Se mantiene constante. */
                double bookMatches,
        /*  palabras por libro. Se mantiene constante. */
                double wordsInBook
) {
       return (bookMatches / wordsInBook);
}
