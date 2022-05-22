#ifndef WORD_H
#define WORD_H
#include "list.h"
#include "treemap.h"


typedef struct Word {
    char word[100];
    double frequency;
    int appearances;
    double relevance;
} Word;

void showWordContext(OrderedTreeMap *map);

#endif
