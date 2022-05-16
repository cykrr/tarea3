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
    //showList (books);


    char in = 0;
    while ( in != 'q') {
        showMenu();
        scanf("%c", &in);
        getchar(); // Eliminar \n del stdin

        switch (in) {
            case 'i':
                books = readBooks();
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
