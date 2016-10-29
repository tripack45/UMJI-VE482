//
// Created by patri on 2016/9/16.
//
#include <stdio.h>
#include "dictionary.h"
#include "libtext_sort.h"
#include "utils.h"

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

    char newFName[20];
    composeFileName(newFName, 20, order, d);

    sortText(iFileName, newFName, d, -1, order);

    return 0;
}

