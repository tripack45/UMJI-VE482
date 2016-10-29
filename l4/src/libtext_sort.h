//
// Created by tripack on 16-10-29.
//

#ifndef VE482_LIBTEXT_SORT_H
#define VE482_LIBTEXT_SORT_H

#include "dictionary.h"

#define LIB_TEXTSORT_BUFSIZE 1000

#define LIB_TEXTSORT_EXCEPTION_FILE_NOT_FOUND -1
#define LIB_TEXTSORT_EXCEPTION_BUFFER_OVERFLOW -2
#define LIB_TEXTSORT_EXCEPTION_FAILED_OUTFILE -3
#define LIB_TEXTSORT_EXCEPTION_UNKNOWN_ORDER -4

typedef enum {
    ORDER_ASCENDING,
    ORDER_RANDOM,
    ORDER_DESCENDING
}ordertype;

int sortText(char* infile,
             char* outfile,
             datatype d,
             ordertype inOrder,
             ordertype outOrder);

#endif //VE482_LIBTEXT_SORT_H
