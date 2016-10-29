//
// Created by patri on 2016/10/17.
//

#ifndef VE482_UTILS_H
#define VE482_UTILS_H

#include "dictionary.h"
#include "libtext_sort.h"

datatype parseFilename(char* fname);

ordertype parseOrder(char* orderName);

void composeFileName(char *buffer, int len,
                     ordertype order, datatype d);

char* cloneSubStr(const char *str, int begin, int end);

datatype parseDatatype(char *p);

#endif //VE482_UTILS_H
