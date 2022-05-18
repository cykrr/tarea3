#ifndef HEAP_h
#define HEAP_h

typedef struct heap Mheap;

Mheap* createMheap();
void* heap_top(Mheap* pq);
void heap_push(Mheap* pq, void* data, int priority);
void heap_pop(Mheap* pq);


#endif /* HashMap_h */

