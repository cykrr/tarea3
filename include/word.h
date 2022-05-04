#ifndef WORD_H
#define WORD_H
#include "list.h"

typedef struct Word {
    char word[20];
    int frequency;
    List *positions;
} Word;
#endif