#include <stdio.h>
#include "treemap.h"
#include "list.h"
#include <string.h>

#include "util.h"
#include "book.h"
#include "menu.h"


int main() {
    List *books = NULL; 
    TreeMap* sortedBooks = createTreeMap(lower_than_string);
    int bookCount = 0;

    //showList (books);

    char in = 0;
    while (1) 
    {
        showMenu();
        scanf("%c", &in);
        getchar(); // Eliminar \n del stdin
        if (in == 'q')
        {
            printf("Gracias por utilizar el programa!");
            return;
        }

        switch (in) {
            case 'i':
                books = readBooks(&bookCount);
                loadBooks(books, sortedBooks);
                break;
            case 'm':
                showBooks(sortedBooks);
                break;
            case 'b':
//                searchBooks(sortedBooks);
                break;
        }
    }

    return 0;
}
