//
// Created by patri on 2016/9/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "exception.h"
#include "utils.h"

#define BUF_SIZE 1000

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Missing Argument, usage "
                       "./ex3 [order]_[type].txt [new order]\n");
        return -1;
    }
    char *iFileName = argv[1];
    char *newOrder = argv[2];
    datatype d = parseFilename(iFileName);
    ordertype order = parseOrder(newOrder);
    FILE *fp = fopen(iFileName, "r");
    if (fp == NULL) {
        printf("Missing input file.\n");
        return -1;
    }

    printf("reading %s\n", iFileName);
    dictionary *dict = new_dictionary();
    char *buf = malloc(BUF_SIZE);
    char c;
    while ((c = readRaw(buf, BUF_SIZE, fp)) != EOF) {
        if (c == 0) {
            printf("Line to long, cannot fit into buffer!\n");
            free(buf); fclose(fp);
            exit(-1);
        }
        keyValuePair *pair = parseLine(buf, d);
        dict->pushBack(dict, pair);
    }
    free(buf);

    printf("sorting elements\n");
    sortfun sort= NULL;
    switch (order) {
        case ORDER_ASCENDING:
            sort = geqKeyValuePair; break;
        case ORDER_RANDOM:
            sort = randKeyValuePair; break;
        case ORDER_DESCENDING:
            sort = randKeyValuePair; break;
        default:
            printf("Unreachable\n"); assert(0);
    }
    sortDictionary(dict, sort);

    char newFName[20];
    composeFileName(newFName, 20, order, d);
    printf("writing %s\n", newFName);
    if (!strcmp(newFName, iFileName)) {
        fclose(fp);
        exit(0); // Nothing to do.
        // This code can be moved upword for better performance
    }
    FILE *nfp = fopen(newFName, "w+");
    if (nfp == NULL) {
        printf("Cannot open output file: %s\n", newFName);
        fclose(fp);
        exit(-1);
    }
    printDictionary(dict, nfp);

    dict->del(dict);
    fclose(nfp);
    fclose(fp);
    return 0;
}
