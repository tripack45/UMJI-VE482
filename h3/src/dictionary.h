//
// Created by patri on 2016/10/17.
//

#ifndef VE482_DICTIONARY_H
#define VE482_DICTIONARY_H

#include "deque.h"
#include <stdio.h>

typedef union {
    int i;
    char c;
    double d;
}valueType;

typedef enum {
    DATA_CHAR, DATA_INT, DATA_DOUBLE
}datatype;

typedef struct keyValuePair_t {
    char* key;
    char* rawVal;
    valueType val;
    datatype type;
    void (*del)(struct keyValuePair_t *obj);
    void (*print)(struct keyValuePair_t *obj, FILE *fp);
}keyValuePair;

keyValuePair *new_keyValuePair(char *key,
                               char *rawVal,
                               valueType val,
                               datatype type);
void keyValuePairDelete(keyValuePair *obj);
void keyValuePairPrint(keyValuePair *obj, FILE *fp);

typedef deque dictionary;
dictionary *new_dictionary();
void dictionaryClear(dictionary* d);

typedef int (*sortfun)(void* x, void* y); // Return 0 if x < y; 1 otherwise

void sortDictionary(dictionary* d, sortfun f);
void printDictionary(dictionary* d, FILE *fp);

int geqKeyValuePair(void* x, void* y);
int ltKeyValuePair(void* x, void* y);
int randKeyValuePair(void* x, void* y);


#endif //VE482_DICTIONARY_H
