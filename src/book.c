#include "book.h"
#include "list.h"
#include "util.h"
#include "word.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


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
createBook(char *id, TreeMap* fileAppearances)
{
    Book *book = malloc(sizeof(Book));
    book->charCount = 0;
    book->wordCount = 0;
    if(!book) 
    {
        printf("Error guardando memoria para Book\n");
    }

    book->wordFrequency = createTreeMap(lower_than_string);
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

    // Avanzar 34 caracteres para leer el titulo.
    for(int i = 0; i < 34; i++) fgetc(book->fd);
    fgets(book->title, 100*sizeof(char), book->fd);

    // Eliminar el Autor si es que existe.
    char *comma = strchr(book->title, ',');
    if(comma != NULL) *comma = '\0';

    // Eliminar salto de linea
    char *newline = strchr(book->title, '\n');
    if(newline != NULL) *newline = '\0';

    // Eliminar salto de linea
    char *cr = strchr(book->title, '\r');
    if(cr != NULL) *cr = '\0';

    countWords(book, fileAppearances);

    return book;
}

void showBooks(TreeMap *sortedMap) 
{
    Pair *bookPair = firstTreeMap(sortedMap);
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
        bookPair = nextTreeMap(sortedMap);
    }
}

void loadBooks(List* books, TreeMap* sortedBooks, TreeMap* fileAppearances, int *count)
{
    char* id = listFirst(books);
    while (id != NULL)
    {
        Book *book = createBook(id, fileAppearances);

        if(book != NULL && searchTreeMap(sortedBooks, book->title) == NULL)  {
            insertTreeMap(sortedBooks, book->title, book);
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
countWords(Book *book, TreeMap* fileAppearances)
{
    TreeMap *exclude = populateExcludeMap();

    /*
    printf("INICIO MAPA\n");
    Pair *aux = firstTreeMap(exclude);
    while (aux != NULL) {
        char *i = aux->value;
        printf("%s\n", i);
        aux = nextTreeMap(exclude);
    }
    printf("FINAL MAPA\n");
    */

    char * x = malloc(1024 * sizeof (char) );
    rewind(book->fd);
    while (fscanf(book->fd, " %1023s", x) == 1) 
    {
        book->charCount += strlen(x);
        (book->wordCount)++;
        quitar_caracteres(x, "[]{}’”“?,.\":;/!-_()\'=*%%");
        stringToLower(x);
     //   printf("%s: ",x );

        if (searchTreeMap(exclude, x) != NULL)
        {
            continue;
        }

      //  printf("\n");

        //printf("%s ", (char*)x);
        Pair *aux = searchTreeMap(book->wordFrequency, x);
        if (aux == NULL)
        {
            Word *word = malloc(sizeof(Word));
            strcpy(word->word, x);
            word->appearances = 1;
            word->positions = listCreate();

            long *toPush = malloc(sizeof(long));
            *toPush = ftell(book->fd);
            listPushBack(word->positions, toPush);

            insertTreeMap(book->wordFrequency, word->word, word);
            //Contar apariciones de una palabra en el archivo
            Pair *tmp = searchTreeMap(fileAppearances, x);
            if (tmp != NULL) 
            {
                *((int*)(tmp->value)) += 1;
            }
            else
            {
                int *insertInt = malloc(sizeof(int));
                *insertInt = 1;
                insertTreeMap(fileAppearances, word->word, insertInt);
            }
        } 
        else 
        {
            ((Word*)(aux->value))->appearances++;
            long *toPush = malloc(sizeof(long));
            *toPush = ftell(book->fd);
            listPushBack(((Word*)(aux->value))->positions, toPush);
        }
   }
}

void 
searchBooks(TreeMap *map) 
{
    char in[50];
    printf("Ingrese la palabra a buscar: ");
    scanf("%s", in);
    getchar();
    Pair *aux = firstTreeMap(map);
    Book *auxBook = NULL;
    Mheap *heap = createMheap();
    //Se recorren los libros
    while (aux != NULL)
    {
        auxBook = aux->value;
        Pair * auxWord = searchTreeMap(auxBook->wordFrequency, in);
        //Se recorren las palabras
        if (auxWord != NULL) 
        {
            heap_push(heap, auxBook, ((Word*)auxWord)->relevance);
        }
        aux = nextTreeMap(map);
    }

    if (heap_top(heap) == NULL)
    {
        printf("Ningun libro contenia la palabra\n");
    }

    while (heap_top(heap)) 
    {
        auxBook = heap_top(heap);
        Pair* auxWord = searchTreeMap(auxBook->wordFrequency, in);
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
}

void getRelevance (TreeMap *map, int totalDocuments, TreeMap* fileAppearances)
{
    Pair *aux = firstTreeMap(map);
    while (aux != NULL) {
//        printf("Libro: %s\n", ((Book*)aux->value)->title);
        Pair *aux2 = firstTreeMap(((Book*)aux->value)->wordFrequency);
        while (aux2 != NULL) {
//            printf("Palabra: %s ", (char*)aux2->key);

            
            Pair *tmp = searchTreeMap(fileAppearances, aux2->key);

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

            aux2 = nextTreeMap(((Book*)aux->value)->wordFrequency);
        }

        aux = nextTreeMap(map);
    }
}

void relevantWords(TreeMap* sortedBooks)
{
    char in[100];
    printf("Ingrese el libro a buscar: ");
    scanf("%[^\n]*s", in);
    getchar();


    /*
    Pair *a = firstTreeMap(sortedBooks);
    int j = 0;
    while (a != NULL) {
        char *i = a->key;
        printf("%d\n %s..\n",j, i);
        j++;

        a = nextTreeMap(sortedBooks);
    }
   
    */

    Pair *aux = searchTreeMap(sortedBooks, in);
    if (aux == NULL)
    {
        printf("El libro que ingreso no existe\n");
        return;
    } else {
        printf("Palabras más relevantes de \'%s\':\n", (char*)aux->key);
    }
    Book *auxBook = aux->value;
    Mheap *heap = createMheap();

    Pair* auxWord = firstTreeMap(auxBook->wordFrequency);
    while (auxWord != NULL) 
    {
        heap_push(heap, auxWord->value, ((Word*)auxWord)->relevance);
        auxWord = nextTreeMap(auxBook->wordFrequency);
    }

    int i = 0;
    while (i < 10 )
    {
        Word *word = heap_top(heap);
        printf("%s: %lf\n", word->word, word->relevance);
        heap_pop(heap);
        i++;
    }
}

//Función que muestra las palabras con mayor frecuencia
void mostFrequency(TreeMap* sortedBooks)
{
    char in[30];
    printf("Ingrese el ID del libro\n");
    scanf("%s", in);
    getchar();
    Pair *bookPair = firstTreeMap(sortedBooks);
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
            Pair *aux = firstTreeMap(book->wordFrequency);
            Mheap *heap = createMheap();

            //Se añaden todas las palabras el Heap para tenerlas ordenadas
            while (aux != NULL) 
            {
                heap_push(heap, aux->value, ((Word*)(aux->value))->frequency );
                aux = nextTreeMap(book->wordFrequency);
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
        bookPair = nextTreeMap(sortedBooks);
    }
    //Caso en donde no se encuentra el ID ingresado.
    printf("El documento que ingreso no existe\n");
}

//Se obtiene la frecuencia de todas las palabras.
void getFrequency(TreeMap* sortedBooks)
{
    Pair *bookPair = firstTreeMap(sortedBooks);
    if (bookPair == NULL) return;
    Book *book;

    //Se recorren los libros leídos
    while (bookPair != NULL)
    {
        book = bookPair->value;
        Pair *auxWrd = firstTreeMap(book->wordFrequency);
        //Se recorren todas las palabras de cada libro y se calcula su frecuencia
        while (auxWrd != NULL)
        {
            ((Word*)(auxWrd->value))->frequency = (double)((Word*)(auxWrd->value))->appearances / (double)book->wordCount;
            auxWrd = nextTreeMap(book->wordFrequency);
        }
        bookPair = nextTreeMap(sortedBooks);
    }
}

//
void bookWithWords(TreeMap* sortedBooks)
{
    char in[100];
    printf("Ingrese las palabras a buscar: ");
    scanf("%[^\n]s", in);
    getchar();
    List *ret = strToList(in, " ");

    Pair *bookPair = firstTreeMap(sortedBooks);
    if (bookPair == NULL)
    {
        printf("No se ha ingresado ningun documento\n");
        return;
    }
    Book *book;
    int flag;
    Pair *auxWrd;
    while (bookPair != NULL)
    {
        flag = 1;
        book = bookPair->value;
        char* words = (char*)listFirst(ret);
        while (words != NULL)
        {
            auxWrd = searchTreeMap(book->wordFrequency, words);
            if (auxWrd == NULL) flag = 0;
            words = (char*)listNext(ret);
        }
        if (flag == 1)
        {
            printf("%s\n", book->title);
        }
        bookPair = nextTreeMap(sortedBooks);
    }
}

TreeMap *
populateExcludeMap() 
{
    TreeMap *excludeMap = createTreeMap(lower_than_string);

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
        "ye", "thee", "thou", "thy", "hath"};
    for (int i = 0; i < sizeof(excludeWords) / (100 * sizeof(char)); i++) {
        char *dup = _strdup(excludeWords[i]);
        insertTreeMap(excludeMap, dup, dup);
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
        /*  coincidencias en el libro. Se mantiene constante. */
                double bookMatches,
        /*  palabras por libro. Se mantiene constante. */
                double wordsInBook
)   {
        double a = frequency(bookMatches, wordsInBook);
        a *= logf(documentCount/totalMatches);
        return a;
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
