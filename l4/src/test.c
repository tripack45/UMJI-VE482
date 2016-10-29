//
// Created by tripack on 16-10-21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dictionary.h"
#include "exception.h"
#include "utils.h"

int main() {
    srand(clock());
    // To test others, change the data and datatype
    dictionary *dict =new_dictionary();
    dict->pushBack(dict, parseLine("abc=123", DATA_INT));
    dict->pushBack(dict, parseLine("xyz=-123", DATA_INT));
    dict->pushBack(dict, parseLine("def=65", DATA_INT));
    dict->pushBack(dict, parseLine("pz2=19", DATA_INT));
    dict->pushBack(dict, parseLine("mx=-500", DATA_INT));
    printf("Original: \n");
    printDictionary(dict, stdout);
    sortDictionary(dict, geqKeyValuePair);
    printf("Asceding: \n");
    printDictionary(dict, stdout);
    dict->del(dict);

    dict =new_dictionary();
    dict->pushBack(dict, parseLine("abc=123", DATA_INT));
    dict->pushBack(dict, parseLine("xyz=-123", DATA_INT));
    dict->pushBack(dict, parseLine("def=65", DATA_INT));
    dict->pushBack(dict, parseLine("pz2=19", DATA_INT));
    dict->pushBack(dict, parseLine("mx=-500", DATA_INT));
    printf("Descending: \n");
    sortDictionary(dict, leqKeyValuePair);
    printDictionary(dict, stdout);
    dict->del(dict);

    dict =new_dictionary();
    dict->pushBack(dict, parseLine("abc=123", DATA_INT));
    dict->pushBack(dict, parseLine("xyz=-123", DATA_INT));
    dict->pushBack(dict, parseLine("def=65", DATA_INT));
    dict->pushBack(dict, parseLine("pz2=19", DATA_INT));
    dict->pushBack(dict, parseLine("mx=-500", DATA_INT));
    printf("Random: \n");
    sortDictionary(dict, randKeyValuePair);
    printDictionary(dict, stdout);
    printf("Random: \n");
    sortDictionary(dict, randKeyValuePair);
    printDictionary(dict, stdout);
    printf("Random: \n");
    sortDictionary(dict, randKeyValuePair);
    printDictionary(dict, stdout);
    dict->del(dict);

    return 0;
}