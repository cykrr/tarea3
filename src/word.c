#include "word.h"
#include "book.h"
#include <stdio.h>
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

    Word *wordInBook = searchTreeMap(book->wordFrequency, word)->value;

    List *positionList = wordInBook->positions;
    for(long *pos = listFirst(positionList); pos != NULL; pos = listNext(positionList)) {
        fseek(book->fd, *pos , SEEK_SET);
        fseek(book->fd, -50 , SEEK_CUR);
        char outWord[50];
        putchar('\n');
        for (int i = 0; i < 20; i ++) {
            fscanf(book->fd, "%s",  outWord);
            printf("%s ", outWord);
        }
        putchar('\n');


    }
}
