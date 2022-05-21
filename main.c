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

    int in = 0;
    while (1) 
    {
        showMenu();
        do
        {
            printf("Ingrese una opcion valida\n");
            scanf("%d", &in);
            getchar(); // Eliminar \n del stdin
        }while(in < 0 || in > 7);

        switch (in) 
        {
            case 0:
            {
                printf("Gracias por utilizar el programa!");
                return 0;
            }
            case 1:
            {
                books = readBooks();
                loadBooks(books, sortedBooks, fileAppearances,
                        &bookCount);
                getRelevance(sortedBooks, bookCount, fileAppearances);
                getFrequency(sortedBooks);
                break;
            }
            case 2:
            {
                showBooks(sortedBooks);
                break;
            }
            case 3:
            {
                bookWithWords(sortedBooks);
                break;
            }
            case 4:
            {
                mostFrequency(sortedBooks);
                break;
            }
            case 5:
            {
                //No esta obteniendo el titulo de manera correcta
                relevantWords(sortedBooks);
                break;
            }
            case 6:
            {
                searchBooks(sortedBooks);
                break;
            }
            case 7:
            {
                break;
            }
        }
    }

    return 0;
}
