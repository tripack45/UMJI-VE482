//
// Created by tripack on 16-9-19.
//

#include "parser.h"
#include "exception.h"
#include "assert.h"
#include "deque.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

stageStack *parse(tokenStack *ts) {
    stageStack *ss= NEW(deque)();
    if (ts->isEmpty(ts)) return ss;
    tokenStack *argStack = NEW(deque)();
    stage *stg = NULL;

    stg = (stage*)malloc(sizeof(stage));
    stg->argv = NULL;
    stg->stdinArg = stg->stdoutArg = NULL;
    stg->stdin = STDIN_NORMAL;
    stg->stdout = STDOUT_NORMAL;
    char *tok = NULL;
    while (!ts->isEmpty(ts)) {
        tok = ts->popFront(ts);
        if (!strcmp(tok, ">")) {
            free(tok);
            if (stg->stdout != STDOUT_NORMAL) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR, NULL);
            }
            stg->stdout = STDOUT_FILE_TRUNCATE;
            if (ts->isEmpty(ts)) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
            }
            tok = ts->popFront(ts);
            if (!strcmp(tok, "|")) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
                free(tok);
            }
            stg->stdoutArg = tok;
        } else if (!strcmp(tok, ">>")) {
            free(tok);
            if (stg->stdout != STDOUT_NORMAL) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR, NULL);
            }
            stg->stdout = STDOUT_FILE_APPEND;
            if (ts->isEmpty(ts)) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
            }
            tok = ts->popFront(ts);
            if (!strcmp(tok, "|")) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
                free(tok);
            }
            stg->stdoutArg = tok;
        } else if (!strcmp(tok, "<")) {
            free(tok);
            if (stg->stdin != STDIN_NORMAL) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_TOO_MANY_STDIN_REDIR, NULL);
            }
            stg->stdin = STDIN_FILE;
            if (ts->isEmpty(ts)) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
            }
            tok = ts->popFront(ts);
            if (!strcmp(tok, "|")) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
                free(tok);
            }
            stg->stdinArg = tok;
        } else if (!strcmp(tok, "|")) {
            free(tok);
            if (stg->stdout != STDOUT_NORMAL) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_REDIRECTION_PIPE_CONFLICT, NULL);
            }
            stg->stdout = STDOUT_PIPED;

            if (argStack->isEmpty(argStack)) {
                ss->del(ss);
                argStack->del(argStack);
                RAISE(EXCEPTION_PASER_MISSING_EXECUTABLE, NULL);
            }

            int argc = argStack->count;
            stg->argv = (char**) malloc(sizeof(char*) * argc);
            stg->argc = argStack->count;
            stg->argv[argc] = NULL; // Required by system call
            for (int i = 0; i < argc; ++i) {
                stg->argv[i] = argStack->popFront(argStack);
            }

            ss->pushBack(ss, stg);
            stg = (stage*)malloc(sizeof(stage));
            stg->argv = NULL;
            stg->stdinArg = stg->stdoutArg = NULL;
            stg->stdin = STDIN_PIPED;
            stg->stdout = STDOUT_NORMAL;
        } else {
            argStack->pushBack(argStack, tok);
        }
    }
    if (argStack->isEmpty(argStack)) {
        ss->del(ss);
        argStack->del(argStack);
        RAISE(EXCEPTION_PASER_MISSING_EXECUTABLE, NULL);
    }
    int argc = argStack->count;
    stg->argv = (char**) malloc(sizeof(char*) * argc);
    stg->argc = argStack->count;
    stg->argv[argc] = NULL; // Required by system call
    for (int i = 0; i < argc; ++i) {
        stg->argv[i] = argStack->popFront(argStack);
    }

    ss->pushBack(ss, stg);

    argStack->del(argStack);

    return ss;
}