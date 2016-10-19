//
// Created by patri on 2016/10/17.
//

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

int isBlank(char c) {
    if (c == '\n') return 1;
    if (c == '\t') return 1;
    return 0;
}

int isSeparator(char c) {
    if (c == '=') return 1;
    return 0;
}

keyValuePair *parseLine(char* line, datatype d) {
    char *p = line;
    while (!isSeparator(*p)){
        if (*p == '\0') assert(0); // '=' not found
        p++;
    }
    int separator = (int) (p - line);
    if (separator == 0) assert(0); // "key" not found
    while (*p != '\0') p++;
    int end = (int) (p - line);
    if (end == separator) assert(0); // "value" not found
    char *k = cloneSubStr(line, 0, separator);
    char *v = cloneSubStr(line, separator + 1, end);
    valueType val;
    switch (d) {
        case DOUBLE: sscanf("%lf", v, &val); break;
        case INT: sscanf("%d", v, &val); break;
        case CHAR: sscanf("%c", v, &val); break;
        default: assert(0);
    }
    keyValuePair *pair = new_keyValuePair(k, val, d);
    free(k); free(v);
    return pair;
}

char* cloneSubStr(const char *str, int begin, int end) {
    int tokenLen = end - begin; // Token Length
    char *tok = malloc(sizeof(char) * (tokenLen + 1)); // '\0'
    memcpy(tok, str + begin, (size_t)tokenLen);
    tok[tokenLen] = '\0';
    return tok;
}