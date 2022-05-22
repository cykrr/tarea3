#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair * pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct OrderedTreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};


TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

OrderedTreeMap * createOrderedTreeMap(int (*lower_than) (void* key1, void* key2)) {
    
    OrderedTreeMap * new = (OrderedTreeMap *)malloc(sizeof(OrderedTreeMap));
    if (new == NULL) return NULL;
    new->root = new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertOrderedTreeMap(OrderedTreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;
    
    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;
    
    while (aux != NULL) {
        parent = aux;
        
        if (tree->lower_than(key,aux->pair->key)) {
            aux = aux->left;
        } else if (tree->lower_than(aux->pair->key,key)) {
            aux = aux->right;
        } else {
            return;
        }
    }
    
    TreeNode * new = createTreeNode(key, value);
    
    new->parent = parent;
    
    if (parent == NULL) {
        tree->root = new;
    } else if (tree->lower_than(key,parent->pair->key)) {
        parent->left = new;
    } else {
        parent->right = new;
    }
    tree->current = new;
}

void removeNode(OrderedTreeMap * tree, TreeNode* node) {
    //nodo hoja
    if(node->right==NULL && node->left==NULL){
        TreeNode* parent=node->parent;
        if(parent == NULL) tree->root=NULL;
        else if(parent->left==node) parent->left=NULL;
        else if(parent->right==node) parent->right=NULL;
    }else if(node->right!=NULL && node->left!=NULL){ //2 hijos
        TreeNode* aux = node->right;
        aux  = minimum(aux);
        node->pair->key=aux->pair->key;
        node->pair->value=aux->pair->value;
        removeNode(tree, aux);
        return;
    }else{ // 1 hijo
        TreeNode* parent=node->parent;
        TreeNode* child = node->right;
        if(child==NULL) child = node->left;
        child->parent = parent;
        if(parent == NULL) tree->root=child;
        else if(parent->left==node) parent->left=child;
        else if(parent->right==node) parent->right=child;
    }
    free(node->pair);
    free(node); 
    return;

}

void eraseOrderedTreeMap(OrderedTreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchOrderedTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchOrderedTreeMap(OrderedTreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode * aux = tree->root;
    
    while (aux != NULL) {
        if (tree->lower_than(key, aux->pair->key)) {
            aux = aux->left;
        } else if (tree->lower_than(aux->pair->key, key)) {
            aux = aux->right;
        }else break;
    }
    
    tree->current = aux;
    
    if (aux == NULL) return NULL;
    
    return (void *)aux->pair;
}


Pair * upperBound(OrderedTreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode * aux = tree->root;
    TreeNode * ub_node = NULL;
    
    while (aux != NULL) {
        if( (ub_node==NULL || tree->lower_than(aux->pair->key, ub_node->pair->key)) &&
           !tree->lower_than(aux->pair->key,key))
           ub_node=aux;

        if (tree->lower_than(key, aux->pair->key)) {
            aux = aux->left;
        } else if (tree->lower_than(aux->pair->key, key)) {
            aux = aux->right;
        }else break;
    }
    
    tree->current = ub_node;

    if (ub_node == NULL) return NULL;
    
    return ub_node->pair;
}

Pair * firstOrderedTreeMap(OrderedTreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    
    TreeNode * aux = tree->root;
    
    tree->current = minimum(aux);
    
    if (tree->current == NULL) return NULL;
    
    return tree->current->pair;
}

Pair * nextOrderedTreeMap(OrderedTreeMap * tree) {
    if (tree == NULL || tree->root == NULL || tree->current == NULL) return NULL;
    
    if (tree->current->right != NULL) {
        TreeNode * aux = tree->current->right;

        tree->current = minimum(aux);
        
        return tree->current->pair;
    }
    
    TreeNode * aux = tree->current->parent;
    
    while (aux != NULL && tree->current == aux->right) {
        tree->current = aux;
        aux = aux->parent;
    }
    
    tree->current = aux;
    
    if (aux == NULL) return NULL;
    
    return aux->pair;
}
