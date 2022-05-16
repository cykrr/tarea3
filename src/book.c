#include "book.h"
#include "list.h"
#include "util.h"
#include "word.h"
#include <stdio.h>
#include <string.h>

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

List * readBooks(int *count) {
    printf("Ingrese los ID de los libros a leer: ");
    char ids[100];

    fflush(stdin);
    scanf("%[^\n]*s", ids);
    getchar();

    List *ret = strToList(ids, " ", count);

    return ret;
}

Book*
createBook(char *id)
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

    countWords(book);

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
        Word *auxWord = aux->value;
        for (int i = 0; i < 10; i++) {
            auxWord = aux->value;
            printf("%s: %d\n", auxWord->word, auxWord->frequency);
            aux = nextTreeMap(book->wordFrequency);
            if (!aux) break;
        }

        bookPair = nextTreeMap(sortedMap);
    }
}

void loadBooks(List* books, TreeMap* sortedBooks)
{
    char* id = listFirst(books);
    while (id != NULL)
    {
        Book *book = createBook(id);

        if(book != NULL) 
            insertTreeMap(sortedBooks, book->title, book);

        id = listNext(books);
    }
}

void 
countWords(Book *book)
{

    char * x = malloc(1024 * sizeof (char) );
    while (fscanf(book->fd, " %1023s", x) == 1) 
    {
        book->charCount += strlen(x);
        book->wordCount++;
        quitar_caracteres(x, "?,.\":;/!-()\'=*%%");
        stringToLower(x);
        Pair *aux = searchTreeMap(book->wordFrequency, x);
        if (aux == NULL)
        {
            Word *word = malloc(sizeof(Word));
            strcpy(word->word, x);
            word->frequency = 1;
            word->positions = NULL; // TODO
            insertTreeMap(book->wordFrequency, word->word, word);
        } 
        else 
        {
            ((Word*)(aux->value))->frequency++;
            printf("%s\n ", (char*)aux->value);
        }
   }
    putchar('\n');
}
