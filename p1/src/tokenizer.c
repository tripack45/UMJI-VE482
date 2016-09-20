//
// Created by tripack on 16-9-18.
//

#include "tokenizer.h"
#include "exception.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

const int MAX_CMD_LEN = 1024;

char* cloneSubStr(const char *str, int begin, int end) {
    int tokenLen = end - begin; // Token Length
    char *tok = malloc(sizeof(char) * (tokenLen + 1)); // '\0'
    memcpy(tok, str + begin, (size_t)tokenLen);
    tok[tokenLen] = '\0';
    return tok;
}

tokenStack *tokenize(const char* str) {
    int len = (int)strlen(str);
    if (len > MAX_CMD_LEN)
        RAISE(EXCEPTION_TOKENIZER_TOO_MANY_CHAR, NULL);
    int stackSize = len / 3;
    stackSize = stackSize < 10 ? 10 : stackSize;
    tokenStack *stack = newDeque(stackSize);
    assert(stack != NULL);
    int begin = 0;
    int end = begin;
    while (str[begin] != '\0') {
        if (isBlank(str[begin]))
            // Skip leading blanks
            begin = getNextNoneBlank(str, begin);
        else {
            char* tok = NULL;
            if (str[begin] == '\"') {
                begin++; // Throw the opening quote
                int nextQuote = getNextQuote(str, begin);
                if (nextQuote == len) {
                    dequeFreeAll(stack);
                    deleteDeque(stack);
                    RAISE(EXCEPTION_TOKENIZER_UNBALANCED_QUOTE, NULL);
                }
                tok = cloneSubStr(str, begin, nextQuote);
                end = nextQuote + 1; // throw the closing quote
            } else {
                end = getNextBlank(str, begin);
                tok = cloneSubStr(str, begin, end);
            }
            dequePushBack(stack, tok);
            CATCH(EXCEPTION_DEQUE_FULL) {
                dequeFreeAll(stack);
                deleteDeque(stack);
                RAISE(EXCEPTION_TOKENIZER_TOO_MANY_TOKEN, NULL);
            }
            begin = end;
        }
    }
    return stack;
}

int getNextNoneBlank(const char* str, int begin) {
    if (str[begin] == '\0')
        return begin;
    int end = begin;
    while (str[end] != '\0') {
        if (!isBlank(str[end])) return end;
        end++;
    }
    assert(str[end] == '\0');
    return end;
}

int getNextQuote(const char* str, int begin) {
    if (str[begin] == '\0')
        return begin;
    int end = begin + 1;
    while (str[end] != '\0') {
        if (str[end] == '\"') return end;
        end++;
    }
    assert(str[end] == '\0');
    return end;
}

int getNextBlank(const char* str, int begin) {
    if (str[begin] == '\0')
        return begin;
    int end = begin;
    while (str[end] != '\0') {
        if (isBlank(str[end])) return end;
        end++;
    }
    assert(str[end] == '\0');
    return end;
}

int isBlank(char c) {
    if (c == ' ') return 1;
    if (c == '\n') return 1;
    if (c == '\t') return 1;
    return 0;
}