#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>
#include "util.h"
#include "book.h"

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
