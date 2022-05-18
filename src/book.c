#include "book.h"
#include "list.h"
#include "util.h"
#include "word.h"
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

Book*
createBook(char *id, TreeMap* fileAppearances)
{
    Book *book = malloc(sizeof(Book));
    book->charCount = 0;
    book->wordCount = 0;
    if(!book) {
        printf("Error guardando memoria para Book\n");
    }

    book->wordFrequency = createTreeMap(inverse_lower_than_string);
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

    countWords(book, fileAppearances);

    return book;
}

void showBooks(TreeMap *sortedMap) 
{
    Pair *bookPair = firstTreeMap(sortedMap);
    Book *book;
    while (bookPair != NULL) 
    {
        book = bookPair->value;

        printf("ID: %s\n", book->id);
        printf("Title: %s\n", book->title);
        printf("Populares: \n");
        Pair *aux = firstTreeMap(book->wordFrequency);
        Word *auxWord;
        if (aux != NULL)
            auxWord = aux->value;
        for (int i = 0; i < 10; i++) {
            if (aux != NULL) {
                auxWord = aux->value;
                printf("%s: %d\n", auxWord->word, auxWord->frequency);
                aux = nextTreeMap(book->wordFrequency);
            }
            if (!aux) break;
        }

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

void 
countWords(Book *book, TreeMap* fileAppearances)
{
    char * x = malloc(1024 * sizeof (char) );
    rewind(book->fd);
    while (fscanf(book->fd, " %1023s", x) == 1) 
    {
        book->charCount += strlen(x);
        book->wordCount++;
        quitar_caracteres(x, "’”“?,.\":;/!-_()\'=*%%");
        stringToLower(x);
        Pair *aux = searchTreeMap(book->wordFrequency, x);
        if (aux == NULL)
        {
            Word *word = malloc(sizeof(Word));
            strcpy(word->word, x);
            word->frequency = 1;
            word->positions = NULL; // TODO
            insertTreeMap(book->wordFrequency, word->word, word);
            //Contar apariciones de una palabra en el archivo
            Pair *tmp = searchTreeMap(fileAppearances, x);
            if (tmp != NULL) 
            {
                ((Word*)(tmp->value))->frequency++;
            }
            else
            {
                Word *word = malloc(sizeof(Word));
                word->frequency = 1;
                insertTreeMap(fileAppearances, word->word, word);
            }
        } 
        else 
        {
            ((Word*)(aux->value))->frequency++;
    //        printf("%s\n ", (char*)aux->value);
        }
   }
    putchar('\n');
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
    //Se recorren los libros
    while (aux != NULL)
    {
        auxBook = aux->value;
        Pair * auxWord = searchTreeMap(auxBook->wordFrequency, in);
        //Se recorren las palabras
        if (auxWord != NULL) 
        {
            showBook(auxBook, auxWord->value);
        }
        aux = nextTreeMap(map);
    }
}

void 
showBook(Book *book, Word *word) 
{
    printf("ID: %s\n", book->id);
    printf("Titulo: %s\n", book->title);
    printf("Apariciones: %d\n", word->frequency);
}

void getRelevance (TreeMap *map, int totalDocuments, TreeMap* fileAppearances)
{
    Pair *aux = firstTreeMap(map);
    Book *auxBook = NULL;
    while (aux != NULL)
    {
        auxBook = aux->value;
        Pair * auxWord = firstTreeMap(auxBook->wordFrequency);
        //Se recorren las palabras
        printf("Segundo while\n");
        if (auxWord != NULL) 
        {
            Pair* tmp = searchTreeMap(fileAppearances, ((Word*)(auxWord->value))->word);
            //int* cont = aa->value;
            printf("Obtener Relevancia\n");
            ((Word*)(auxWord->value))->relevance = 
                (
                 ((float)((Word*)(auxWord->value))->frequency) / auxBook->wordCount
                ) * log((float)totalDocuments / 
                     ((Word*)(tmp->value))->frequency);

            auxWord = nextTreeMap(auxBook->wordFrequency);
        }
        aux = nextTreeMap(map);
    }
}
