//
// Created by tripack on 16-10-29.
//

#include "libtext_sort.h"
#include "utils.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

keyValuePair *parseLine(char* str, datatype d);
char readRaw(char* buf, int bufLen, FILE* fp);

int sortText(char* infile,
             char* outfile,
             datatype d,
             ordertype inOrder,
             ordertype outOrder) {

    srand(clock());
    FILE *fp = fopen(infile, "r");
    if (fp == NULL) {
        return LIB_TEXTSORT_EXCEPTION_FILE_NOT_FOUND;
    }

    FILE *nfp = fopen(outfile, "w+");
    if (nfp == NULL) {
        return LIB_TEXTSORT_EXCEPTION_FAILED_OUTFILE;
    }

    printf("reading %s\n", infile);
    dictionary *dict = new_dictionary();
    char *buf = malloc(LIB_TEXTSORT_BUFSIZE);
    char c;
    while ((c = readRaw(buf, LIB_TEXTSORT_BUFSIZE, fp)) != EOF) {
        if (c == 0) {
            free(buf); fclose(fp); fclose(nfp);
            return LIB_TEXTSORT_EXCEPTION_BUFFER_OVERFLOW;
        }
        keyValuePair *pair = parseLine(buf, d);
        dict->pushBack(dict, pair);
    }
    free(buf);
    fclose(fp);

    printf("sorting elements\n");
    sortfun sort= NULL;
    switch (outOrder) {
        case ORDER_ASCENDING:
            sort = geqKeyValuePair; break;
        case ORDER_RANDOM:
            sort = randKeyValuePair; break;
        case ORDER_DESCENDING:
            sort = leqKeyValuePair; break;
        default:
            return LIB_TEXTSORT_EXCEPTION_UNKNOWN_ORDER;
    }
    sortDictionary(dict, sort);

    printDictionary(dict, nfp);

    dict->del(dict);
    fclose(nfp);
    return 0;
}

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
        case DATA_DOUBLE: sscanf(v, "%lf", &(val.d)); break;
        case DATA_INT: sscanf(v, "%d", &(val.i)); break;
        case DATA_CHAR: sscanf(v, "%c", &(val.c)); break;
        default: assert(0);
    }
    keyValuePair *pair = new_keyValuePair(k, v, val, d);
    free(k); free(v);
    return pair;
}

char readRaw(char* buf, int bufLen, FILE *fp) {
    int count = 0;
    while (count < bufLen) {
        char c = (char)fgetc(fp);
        //printf("== [%d] ==", c);
        switch (c) {
            case '\n':
            case '\r':
            case EOF: { // Terminaing characters get
                buf[count] = '\0';
                return c;
            }
            default: // Usual characters
                buf[count] = c;
        }
        count++;
    }
    buf[count] = '\0';
    return 0; // Buffer full return.
}