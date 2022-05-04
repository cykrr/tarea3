#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>
#include "util.h"
#include "book.h"

Book *createBook(char *id) {
    Book *book = malloc(sizeof(Book));
    if(!book) {
        printf("Error guardando memoria para Book\n");
    }

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

    for(int i = 0; i < 34; i++) fgetc(book->fd);
    fgets(book->title, 100*sizeof(char), book->fd);

    char *comma = strchr(book->title, ',');
    if(comma != NULL) *comma = '\0';

    return book;
}

void loadBooks(List* books, TreeMap* sortedBooks)
{
    char* id = listFirst(books);
    while (id != NULL)
    {
        
    }
}

int main() {
    List *books = readBooks();
    TreeMap* sortedBooks = createTreeMap(lower_than_string);
    //TreeMap* wordFrecuency = createTreeMap(lower_than_string);
    showList (books);
    loadBooks(books, sortedBooks);

    return 0;
}
