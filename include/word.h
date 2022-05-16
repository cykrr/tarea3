#ifndef WORD_H
#define WORD_H
#include "list.h"

typedef struct Word {
    char word[100];
    int frequency;
    List *positions;
} Word;
#endif
