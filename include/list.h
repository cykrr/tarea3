#ifndef LIST_H
#define LIST_H
#include <stdlib.h>


struct Node {
    void * data;
    struct Node * next;
    struct Node * prev;
};

typedef struct Node Node;

typedef struct List List;

struct List {
    Node * head;
    Node * tail;
    Node * current;
    size_t length;
};


Node * nodeCreate(void * data);
List * listCreate();

void * listFirst(List * list);
void * listNext(List * list);
void * listLast(List * list);
void * listPrev(List * list);

void listPushBack(List * list, void * data);
void listPushFront(List * list, void * data);
void listPushCurrent(List * list, void * data);

void * listPopFront(List * list);
void * listPopBack(List * list);
void * listPopCurrent(List * list);

void listClean(List * list);
#endif
