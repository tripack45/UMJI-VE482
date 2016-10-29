//
// Created by patri on 2016/10/17.
//

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"




char* cloneSubStr(const char *str, int begin, int end) {
    int tokenLen = end - begin; // Token Length
    char *tok = malloc(sizeof(char) * (tokenLen + 1)); // '\0'
    memcpy(tok, str + begin, (size_t)tokenLen);
    tok[tokenLen] = '\0';
    return tok;
}

// We actually don't care the original file order!
// We can do this without this information!
datatype parseFilename(char* fname) {
    const char separator = '_';
    assert(fname != NULL);
    char *p = fname;
    while (*p != '\0' && *p != separator) p++;
    if (*p == '\0') {
        printf("Invalid Filename!\n");
        exit(-1);
    }
    p++; // Consume the separator
    if (*p == '\0') {
        printf("Missing Datatype!\n");
        exit(-1);
    }
    if (!strcmp(p,"int.txt"))    return DATA_INT;
    if (!strcmp(p,"double.txt")) return DATA_DOUBLE;
    if (!strcmp(p,"char.txt"))   return DATA_CHAR;
    printf("Unrecogonized Datatype!");
    exit(-1);
}

ordertype parseOrder(char* orderName) {
    assert(orderName != NULL);
    if (!strcmp(orderName, "inc")) return ORDER_ASCENDING;
    if (!strcmp(orderName, "rand")) return ORDER_RANDOM;
    if (!strcmp(orderName, "dec")) return ORDER_DESCENDING;
    printf("Unrecogonized Order!\n");
    exit(-1);
}

void composeFileName(char *buffer, int len,
                     ordertype order, datatype d) {
    char buf[20] = "";
    char *orderStr = NULL;
    char *dataTypeStr = NULL;
    switch (order) {
        case ORDER_ASCENDING: orderStr = "inc"; break;
        case ORDER_DESCENDING: orderStr = "dec"; break;
        case ORDER_RANDOM: orderStr = "rand"; break;
        default:assert(0);
    }
    switch (d) {
        case DATA_INT: dataTypeStr = "int"; break;
        case DATA_DOUBLE: dataTypeStr = "double"; break;
        case DATA_CHAR: dataTypeStr = "char"; break;
        default:assert(0);
    }
    strcat(buf, orderStr);
    strcat(buf, "_");
    strcat(buf, dataTypeStr);
    strcat(buf, ".txt");
    if (strlen(buf) > len) { // Defensive on overflow
        printf("Internal Error!\n");
        exit(-1);
    }
    strcpy(buffer, buf);
}