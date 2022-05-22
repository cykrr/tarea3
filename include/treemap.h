#ifndef TREEMAP_h
#define TREEMAP_h

typedef struct OrderedTreeMap OrderedTreeMap;

typedef struct Pair {
     void * key;
     void * value;
} Pair;

OrderedTreeMap * createOrderedTreeMap(int (*lower_than_int) (void* key1, void* key2));

void insertOrderedTreeMap(OrderedTreeMap * tree, void* key, void * value);

void eraseOrderedTreeMap(OrderedTreeMap * tree, void* key);

Pair * searchOrderedTreeMap(OrderedTreeMap * tree, void* key);

Pair * upperBound(OrderedTreeMap * tree, void* key);

Pair * firstOrderedTreeMap(OrderedTreeMap * tree);

Pair * nextOrderedTreeMap(OrderedTreeMap * tree);

#endif /* TREEMAP_h */
