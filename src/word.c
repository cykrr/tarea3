#include "word.h"
#include "book.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
void 
showWordContext(OrderedTreeMap *bookMap) 
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
    Book *book = searchOrderedTreeMap(bookMap, str)->value;
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

void setWordRelevance(Word *word, double documentCount, double totalMatches) {
    word->relevance = relevance(documentCount, totalMatches, word->frequency);
}

double 
relevance
(
        /* la cantidad de documentos. Se actualiza cada vez que
         * se añade un libro. */
                float documentCount,
        /*  coincidencias en todos los libros. Se actualiza cada vez
         *  que se añade un libro. */
                float totalMatches, 
        /* frecuencia de la palabra en el libro */
                float frequency
)   {
        
        return frequency *= logf(documentCount/totalMatches);
}

double 
frequency( 
        /*  coincidencias en el libro. Se mantiene constante. */
                double bookMatches,
        /*  palabras por libro. Se mantiene constante. */
                double wordsInBook
) {
       return (bookMatches / wordsInBook);
}

OrderedTreeMap *
populateExcludeMap() 
{
    OrderedTreeMap *excludeMap = createOrderedTreeMap(lower_than_string);

    char excludeWords[][100] = {"the", "and", "of", "to", "that", 
        "in", "he", "shall", "unto", "for", "a", "was", "it",
        "she", "said", "you", "be", "an", "have", "i", "not",
        "on", "with", "as", "do", "at", "this", "but", "his",
        "by", "from", "they", "we", "say", "her", "or", "will",
        "my", "one", "all", "would", "there", "their", "what", 
        "so", "up", "out", "if", "about", "who", "get", "which",
        "go", "me", "when", "make", "can", "like", "no", "just",
        "him", "know", "take", "into", "your", "good", "some", 
        "could", "them", "than", "then", "now", "come", "its",
        "also", "how", "our", "well", "even", "want", "because",
        "any", "most", "us", "are", "is", "had", "were", "went", 
        "ye", "thee", "thou", "thy", "hath", "has", "may", "more"};
    int count = sizeof(excludeWords) / (100 * sizeof(char));
    //Se insertan las palabras al mapa de exclusion
    for (int i = 0; i < count; i++) {
        char *dup = _strdup(excludeWords[i]);
        insertOrderedTreeMap(excludeMap, dup, dup);
    }
    return excludeMap;
}
