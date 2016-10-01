//
// Created by tripack on 16-9-18.
//

#ifndef VE482_TOKENIZER_H
#define VE482_TOKENIZER_H

#include "deque.h"


#define TOKEN_PIPE 0x00d1
#define TOKEN_REDIR_STDIN 0x00d2
#define TOKEN_REDIR_STDOUT_TRUNC 0x00d3
#define TOKEN_REDIR_STDOUT_APPEND 0x00d4
#define TOKEN_STRING 0x00f1

typedef struct token_t{
    int type;
    char* content;

    char* (*cloneContent)(struct token_t *obj);
    void (*del)(struct token_t *obj);
}token;

token *new_token(int type, char* content);
#ifdef TOKENIZER_EXPOSE_PRIVATE
void tokenDelete(token *obj);
char* tokenCloneContent(token *obj);
#endif


typedef deque tokenStack;
tokenStack *new_tokenStack();
#ifdef TOKENIZER_EXPOSE_PRIVATE
void clearTokenStack(tokenStack *obj);
#endif

int getNextNoneBlank(const char* str, int begin);
int getNextBlank(const char* str, int begin);
int getNextQuote(const char* str, int begin);

int isOperator(int type);
int isBlank(char c);
int getTokenType(char* str);

void tokenize(const char* str, tokenStack *ts);

char readRaw(char* buf, int bufLen);

#endif //VE482_TOKENIZER_H
