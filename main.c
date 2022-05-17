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
    TreeMap* fileAppearances = createTreeMap(lower_than_string); //Guardara las apariciones de las palabras en todos los archivos
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
            return 0;
        }

        switch (in) {
            case 'i':
            {
                books = readBooks(&bookCount);
                loadBooks(books, sortedBooks, fileAppearances);
                break;
            }
            case 'm':
            {
                showBooks(sortedBooks);
                break;
            }
            case 'b':
            {
                searchBooks(sortedBooks);
                break;
            }
            case 'r':
            {
                getRelevance(sortedBooks, bookCount, fileAppearances);
            }
        }
    }

    return 0;
}
