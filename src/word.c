#include "word.h"
#include "book.h"
#include <stdio.h>
#include <string.h>
void 
showWordContext(TreeMap *bookMap) 
{
    char str[101];
    char word[20];
    printf("Ingrese el título del libro: ");
    fflush(stdin);
    scanf("%[^\n]*s", str);
    getchar();
    fflush(stdin);
    printf("Ingrese la palabra a buscar");
    scanf("%[^\n]*s", word);
    getchar();
    Book *book = searchTreeMap(bookMap, str)->value;
    char line[1024];
    char lineLower[1024];
    rewind(book->fd);
    while (fgets(line, 1023, book->fd))
    {
        strcpy(lineLower, line);
        stringToLower(lineLower);

        if (strstr(lineLower, word) != 0)
        {
            printf("%s\n", line);
        }
    }   
}
