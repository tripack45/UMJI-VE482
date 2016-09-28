//
// Created by tripack on 16-9-18.
//
#define TOKENIZER_EXPOSE_PRIVATE

#include "tokenizer.h"
#include "exception.h"
#include "oop.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

const int MAX_CMD_LEN = 1024;


/* Clone the string "str" from "begin" to "end"
 * Return the cloned string
 * Needs to be freed after used
 */
char* cloneSubStr(const char *str, int begin, int end) {
    int tokenLen = end - begin; // Token Length
    char *tok = malloc(sizeof(char) * (tokenLen + 1)); // '\0'
    memcpy(tok, str + begin, (size_t)tokenLen);
    tok[tokenLen] = '\0';
    return tok;
}


/* Tokenize a string "str"
 * Append the tokens into ts
 * Requires both arguments are not NULL
 *
 * Each token has a type.
 * String token has content
 * this content needs to be freed after used.
 * returns a newly build "tokenStack".
 * Needs to be freed after used.
 */

void tokenize(const char* str, tokenStack *ts) {
    assert(str != NULL);
    assert(ts != NULL);

    int len = (int)strlen(str);
    if (len > MAX_CMD_LEN)
        RAISE_VOID(EXCEPTION_TOKENIZER_TOO_MANY_CHAR);

    int begin = 0;
    int end = begin;
    while (str[begin] != '\0') {
        if (isBlank(str[begin]))
            // Skip leading blanks
            begin = getNextNoneBlank(str, begin);
        else {
            token *tok = NULL;
            if (str[begin] == '\"') {
                begin++; // Throw the opening quote
                int nextQuote = getNextQuote(str, begin);
                if (nextQuote == len) {
                    ts->del(ts);
                    RAISE_VOID(EXCEPTION_TOKENIZER_UNBALANCED_QUOTE);
                }
                int type = TOKEN_STRING;
                char* content = cloneSubStr(str, begin, nextQuote);
                tok = NEW(token)(type, content);
                end = nextQuote + 1; // throw the closing quote
            } else {
                end = getNextBlank(str, begin);
                char *content = cloneSubStr(str, begin, end);
                int type = getTokenType(content);
                if (isOperator(type)) {
                    free(content);
                    tok = NEW(token)(type, NULL);
                } else {
                    tok = NEW(token)(type, content);
                }
            }
            ts->pushBack(ts, tok);
            begin = end;
        }
    }
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

token *new_tok(int type, char* content) {
    token *tok = malloc(sizeof(token));
    tok->type = type;
    tok->content = content;
    return tok;
}

int getTokenType(char* str) {
    if (strcmp(str, "|") == 0) return TOKEN_PIPE;
    if (strcmp(str, "<") == 0) return TOKEN_REDIR_STDIN;
    if (strcmp(str, "<") == 0) return TOKEN_REDIR_STDOUT_TRUNC;
    if (strcmp(str, "<<") == 0) return TOKEN_REDIR_STDOUT_APPEND;
    return TOKEN_STRING;
}

int isOperator(int type) {
    return (type != TOKEN_STRING);
}



tokenStack *new_tokenStack() {
    deque *dq = NEW(deque)();
    // Behold! The majesty of Polymorphism!
    dq->clear = clearTokenStack;
}

void clearTokenStack(tokenStack *obj) {
    while (!obj->isEmpty(obj)) {
        token *tok = obj->popFront(obj);
        tok->del(tok);
    }
    free(obj);
}

token *new_token(int type, char* content) {
    token *tok = malloc(sizeof(token));
    tok->del = tokenDelete;

    tok->type = type;
    tok->content = content;
}

void tokenDelete(token *obj) {
    if (obj->content != NULL)
        free(obj->content);
    free(obj);
}

