//
// Created by patri on 2016/10/17.
//

#include <stdlib.h>
#include "dictionary.h"
#include <string.h>
#include <assert.h>

keyValuePair *new_keyValuePair(char *key, char *rawVal,
                               valueType val, datatype type) {
    assert(key != NULL);
    assert(rawVal != NULL);
    keyValuePair *pair = malloc(sizeof(keyValuePair));

    size_t len = strlen(key);
    size_t size = len + 1;
    pair->key = malloc(size);
    strcpy(pair->key, key);

    len = strlen(rawVal);
    size = len + 1;
    pair->rawVal = malloc(size);
    strcpy(pair->rawVal, rawVal);

    pair->val = val;
    pair->type = type;

    pair->del = keyValuePairDelete;
    pair->print = keyValuePairPrint;
    return pair;
}

void keyValuePairDelete(keyValuePair *obj) {
    if (obj->key != NULL) free(obj->key);
    if (obj->rawVal != NULL) free(obj->rawVal);
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
        for (node *jnode = end->prev; jnode != begin; jnode = jnode->prev) {
            if (!f(jnode->value, jnode->prev->value)) { // if not (prev <= next), sorts to ascending
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
        case DATA_CHAR: return a->val.c >= b->val.c;
        case DATA_DOUBLE: return a->val.d >= b->val.d;
        case DATA_INT: return a->val.i >= b->val.i;
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

void printDictionary(dictionary* d, FILE *fp) {
    node *begin = d->head.next;
    node *end   = &(d->tail);
    for (node *n = begin; n != end; n = n->next) {
        keyValuePair *pair = n->value;
        pair->print(pair, fp);
        fprintf(fp, "\n"); // One Pair per line.
    }
}

void keyValuePairPrint(keyValuePair *obj, FILE *fp) {
    fprintf(fp, "%s", obj->key);
    fprintf(fp, "=");
    fprintf(fp, "%s", obj->rawVal);
}