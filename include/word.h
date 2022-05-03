#ifndef WORD_H
#define WORD_H
typedef struct Word {
    char word[20];
    int frequency;
    List *positions;
};
#endif