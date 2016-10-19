//
// Created by patri on 2016/10/17.
//

#ifndef VE482_DICTIONARY_H
#define VE482_DICTIONARY_H

#include "deque.h"

typedef union {
    int i;
    char c;
    double d;
}valueType;

typedef enum {
    CHAR, INT, DOUBLE
}datatype;

typedef struct keyValuePair_t {
    char* key;
    valueType val;
    datatype type;
    void (*del)(struct keyValuePair_t *obj);
}keyValuePair;

keyValuePair *new_keyValuePair(char *key,
                               valueType val,
                               datatype type);
void keyValuePairDelete(keyValuePair *obj);

typedef deque dictionary;
dictionary *new_dictionary();
void dictionaryClear(dictionary* d);

typedef int (*sortfun)(void* x, void* y); // Return 0 if x < y; 1 otherwise

void sortDictionary(dictionary* d, sortfun f);


#endif //VE482_DICTIONARY_H
