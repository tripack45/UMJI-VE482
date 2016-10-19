//
// Created by patri on 2016/10/17.
//

#ifndef VE482_UTILS_H
#define VE482_UTILS_H

#include "dictionary.h"

typedef enum {
    ORDER_ASCENDING,
    ORDER_RANDOM,
    ORDER_DESCENDING
}ordertype;

keyValuePair *parseLine(char* str, datatype d);

datatype parseFilename(char* fname);

ordertype parseOrder(char* orderName);

void composeFileName(char *buffer, int len,
                     ordertype order, datatype d);

char* cloneSubStr(const char *str, int begin, int end);

char readRaw(char* buf, int bufLen, FILE* fp);
#endif //VE482_UTILS_H
