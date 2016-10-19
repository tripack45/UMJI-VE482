//
// Created by patri on 2016/10/17.
//

#include <stdlib.h>
#include "dictionary.h"
#include <string.h>
#include <assert.h>

keyValuePair *new_keyValuePair(char *key, valueType val, datatype type) {
    assert(key != NULL);
    int len = strlen(key);
    int size = len + 1;
    keyValuePair *pair = malloc(sizeof(keyValuePair));
    pair->key = malloc(size);
    strcpy(pair->key, key);
    pair->val = val;
    pair->del = keyValuePairDelete;
    return pair;
}

void keyValuePairDelete(keyValuePair *obj) {
    if (obj->key != NULL) free(obj->key);
    free(obj);
}

dictionary *new_dictionary() {
    deque *dq = new_deque();
    dq->clear=dictionaryClear;
    return dq;
}

void dictionaryClear(dictionary* d) {
    while (!d->isEmpty(d)) {
        keyValuePair *pair = d->popFront(d);
        pair->del(pair);
    }
}

void sortDictionary(dictionary* d, sortfun f) {
    // A bubble sorting algorithm applied to the ADT
    node *begin = d->head.next;
    node *end = &d->tail;
    for (node *inode = begin; inode != end; inode = inode->next) {
        for (node *jnode = end; jnode != inode; jnode = jnode->prev) {
            if (!f(jnode->prev, jnode)) { // if not (prev <= next), sorts to ascending
                node tnode;
                tnode.value = jnode->value;
                jnode->value = jnode->prev->value;
                jnode->prev->value = tnode.value;
            }
        }
    }
}

int geqKeyValuePair(void* x, void* y) {
    keyValuePair *a = x;
    keyValuePair *b = y;
    assert(x != NULL);
    assert(y != NULL);
    switch (a->type) {
        case CHAR: return a->val.c >= b->val.c;
        case DOUBLE: return a->val.d >= b->val.d;
        case INT: return a->val.i >= b->val.i;
        default: assert(0); // This shouldn't happen
    }
    return -1;
}

int ltKeyValuePair(void* x, void* y) {
    return !geqKeyValuePair(x, y);
}

int randKeyValuePair(void* x, void* y) {
    return rand() & 1;
}