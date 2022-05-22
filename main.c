#include <stdio.h>
#include <string.h>

#include "util.h"
#include "book.h"
#include "menu.h"

#include "treemap.h"
#include "hashmap.h"
#include "list.h"

// Punto de entrada el programa

int main() {
    List *books = NULL;  // Almacena la lista de libros a cargar.
    // Mapa ordenado para almacenar los libros y ordenarlos por nombre
    OrderedTreeMap* sortedBooks = createOrderedTreeMap(lower_than_string); 
    /* HashMap para almacenar las apariciones de todas las palabras en todos
     * los archivos */ 
    HashMapSus* fileAppearances = createMapSus(100);
    // Almacena la cantidad de libros cargados por el programa
    int bookCount = 0;

    int in = 1; // almacena la entrada
    while (1) 
    {

        showMenu(); // Muestra las opciones del menú disponibles

        do
        {
            // Solo mostrar advertencia de ser necesario
            if (in < 0 || in > 7)
                printf("Ingrese una opcion valida\n");
            scanf("%d", &in);
            getchar(); // Eliminar \n del stdin
                       //
        }while(in < 0 || in > 7);

        switch (in) 
        {
            case 0: // Salir del programa
            {
                printf("Gracias por utilizar el programa!");
                return 0;
            }
            case 1: // Importar libros
            {
                books = readBooks();
                loadBooks(books, sortedBooks, fileAppearances,
                        &bookCount);
                break;
            }
            case 2: // Mostrar libros
            {
                showBooks(sortedBooks);
                break;
            }
            case 3: // Buscar libro por coincidencias
            {
                bookWithWords(sortedBooks);
                break;
            }
            case 4: // Palabras con mayor frecuencia
            {
                mostFrequency(sortedBooks);
                break;
            }
            case 5: // Palabras más relevantes
            {
                relevantWords(sortedBooks, fileAppearances, bookCount);
                break;
            }
            case 6: // Buscar libros por palabra
            {
                searchBooks(sortedBooks, bookCount, fileAppearances);
                break;
            }
            case 7: // Mostrar palabra en contexto
            {
                showWordContext(sortedBooks);
                break;
            }
        }
    }

}
