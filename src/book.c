#include "book.h"
#include "list.h"
#include "util.h"
#include "word.h"
#include <stdio.h>
#include <string.h>

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

Book *createBook(char *id) {
    Book *book = malloc(sizeof(Book));
    book->charCount = 0;
    if(!book) {
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
        quitar_caracteres(x, "?,.\":;/!-()\'=*%%");
        stringToLower(x);
        Pair *aux = searchTreeMap(book->wordFrequency, x);
        if (aux == NULL)
        {
            Word *word = malloc(sizeof(Word));
            strcpy(word->word, x);
            word->frequency = 1;
            word->positions = NULL; // TODO
            insertTreeMap(book->wordFrequency, x, word);
        } 
        else 
        {
            ((Word*)(aux->value))->frequency++;
        }
        printf("%s ", x);
   }
    putchar('\n');
}
