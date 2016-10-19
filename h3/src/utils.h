//
// Created by patri on 2016/10/17.
//

#ifndef VE482_UTILS_H
#define VE482_UTILS_H

#include "dictionary.h"

keyValuePair *parseLine(char* str, datatype d);

char* cloneSubStr(const char *str, int begin, int end);
#endif //VE482_UTILS_H
