//
// Created by tripack on 16-9-18.
//

#ifndef VE482_TOKENIZER_H
#define VE482_TOKENIZER_H

#include "deque.h"

typedef deque tokenStack;


tokenStack *tokenize(const char* str);

int getNextNoneBlank(const char* str, int begin);
int getNextBlank(const char* str, int begin);
int getNextQuote(const char* str, int begin);

int isBlank(char c);



#endif //VE482_TOKENIZER_H
