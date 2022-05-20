#ifndef WORD_H
#define WORD_H
#include "list.h"

typedef struct Word {
    char word[100];
    double frequency;
    int appearances;
    double relevance;
    List *positions;
} Word;
#endif
