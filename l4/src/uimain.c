//
// Created by tripack on 16-10-29.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define INPUT_BUF_SIZE 100

void getStrInput(char* buf, int bufSize);
int menu();
int entrySort();
int entryDisplay();
int entryExit();

const char* entryText[] = {
        "Sort a file\n",
        "Print Content of a file\n",
        "Exit Program\n"
};

int (*(entry[]))() = {
        entrySort,
        entryDisplay,
        entryExit
};

int main() {
    printf("Textual dictionary sorter v0.2\n");
    for(;;) {
        int i = menu();
        int ret = (entry[i])();
    }
}

int menu() {
    printf("Please select a function.\n");
    int len = sizeof(entry) / sizeof(*entry);
    for (int i = 0; i < len; ++i) {
        printf("%d) %s", i, entryText[i]);
    }
    printf("Choice: ");
    int choice; scanf("%d", &choice);
    while (choice >= len || choice < 0) {
        printf("Invalid choice, please retry: ");
        scanf("%d", &choice);
    }
    return choice;
}

int entrySort() {
    char iFileName[INPUT_BUF_SIZE];
    printf("Input the file you would like to sort: ");
    getStrInput(iFileName, INPUT_BUF_SIZE);

    printf("\nThe file can have one of the following 3 datatype:\n");
    printf("int\tinteger of size %lu bytes\n", sizeof(int));
    printf("double\tdouble precision float of %lu bytes\n", sizeof(double));
    printf("char\tthe value is a single ascii character\n");

    datatype d;
    printf("\nPlease input the data type: ");
    char datatype[INPUT_BUF_SIZE];
    for(;;) {
        getStrInput(datatype, INPUT_BUF_SIZE);
        d = parseDatatype(datatype);
        if ((int)d >= 0) break;
        printf("Retry input: ");
    }

    printf("\nThe file can have one of the following 3 orders:\n");
    printf("inc\tIncreasing order\n");
    printf("dec\tDecreasing order\n");
    printf("rand\tRandom order\n");

    ordertype inOrder;
    printf("\nPlease input the order of the input file: ");
    char newOrder[INPUT_BUF_SIZE];
    for(;;) {
        getStrInput(newOrder, INPUT_BUF_SIZE);
        inOrder = parseOrder(newOrder);
        if ((int)inOrder >= 0) break;
        printf("Retry input: ");
    }

    ordertype outOrder;
    printf("\nPlease input the order of the output file: ");
    for(;;) {
        getStrInput(newOrder, INPUT_BUF_SIZE);
        outOrder = parseOrder(newOrder);
        if ((int)outOrder >= 0) break;
        printf("Retry input: ");
    }

    char newFName[100];
    composeFileName(newFName, 100, outOrder, d);

    printf("\nInput file name: %s\n", iFileName);
    printf("Output file name: %s\n\n", newFName);

    int ret = sortText(iFileName, newFName, d, -1, outOrder);
    switch (ret) {
        case 0:
            break;
        case LIB_TEXTSORT_EXCEPTION_FILE_NOT_FOUND:
            printf("Input file not found!\n");
            break;
        case LIB_TEXTSORT_EXCEPTION_FAILED_OUTFILE:
            printf("Cannot open the output file!\n");
            break;
        case LIB_TEXTSORT_EXCEPTION_BUFFER_OVERFLOW:
            printf("File contains a line that is too long for buffer.");
            break;
        default:
            printf("%d, Unknown other error.\n", ret);
    }

    return -1;
}

int entryExit() {
    exit(0);
}

int entryDisplay() {
    char iFileName[INPUT_BUF_SIZE];
    printf("Input the file you would like to print: ");
    getStrInput(iFileName, INPUT_BUF_SIZE);
    FILE* fp = fopen(iFileName, "r");
    if (fp == NULL) {
        printf("Cannot open file \"%s\"\n", iFileName);
        return -1;
    }
    putchar('\n');
    char c;
    while ((c=fgetc(fp)) != EOF) putchar(c);
    putchar('\n');
}


void getStrInput(char* buf, int bufSize) {
    char *nbuf = malloc(bufSize);
    for (;;){
        *nbuf = '\0';
        fgets(nbuf, bufSize, stdin);
        if (sscanf(nbuf, "%s", buf) > 0) break;
    }
    free(nbuf);
}