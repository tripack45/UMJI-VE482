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
    stageStack *ss= newDeque(30);
    if (DEQUE_ISEMPTY(ts)) return ss;
    tokenStack *argStack = newDeque(50);
    stage *stg = NULL;

    stg = (stage*)malloc(sizeof(stage));
    stg->argv = NULL;
    stg->stdinArg = stg->stdoutArg = NULL;
    stg->stdin = STDIN_NORMAL;
    stg->stdout = STDOUT_NORMAL;
    char *tok = NULL;
    while (!DEQUE_ISEMPTY(ts)) {
        tok = dequePopFront(ts);
        if (!strcmp(tok, ">")) {
            free(tok);
            if (stg->stdout != STDOUT_NORMAL) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR, NULL);
            }
            stg->stdout = STDOUT_FILE_TRUNCATE;
            if (DEQUE_ISEMPTY(ts)) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
            }
            tok = dequePopFront(ts);
            if (!strcmp(tok, "|")) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
                free(tok);
            }
            stg->stdoutArg = tok;
        } else if (!strcmp(tok, ">>")) {
            free(tok);
            if (stg->stdout != STDOUT_NORMAL) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR, NULL);
            }
            stg->stdout = STDOUT_FILE_APPEND;
            if (DEQUE_ISEMPTY(ts)) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
            }
            tok = dequePopFront(ts);
            if (!strcmp(tok, "|")) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
                free(tok);
            }
            stg->stdoutArg = tok;
        } else if (!strcmp(tok, "<")) {
            free(tok);
            if (stg->stdin != STDIN_NORMAL) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_TOO_MANY_STDIN_REDIR, NULL);
            }
            stg->stdin = STDIN_FILE;
            if (DEQUE_ISEMPTY(ts)) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
            }
            tok = dequePopFront(ts);
            if (!strcmp(tok, "|")) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_REDIRECTION_FILE, NULL);
                free(tok);
            }
            stg->stdinArg = tok;
        } else if (!strcmp(tok, "|")) {
            free(tok);
            if (stg->stdout != STDOUT_NORMAL) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_REDIRECTION_PIPE_CONFLICT, NULL);
            }
            stg->stdout = STDOUT_PIPED;

            if (DEQUE_ISEMPTY(argStack)) {
                dequeFreeAll(ss); deleteDeque(ss);
                dequeFreeAll(argStack); deleteDeque(argStack);
                RAISE(EXCEPTION_PASER_MISSING_EXECUTABLE, NULL);
            }

            int argc = argStack->count;
            stg->argv = (char**) malloc(sizeof(char*) * argc);
            stg->argc = argStack->count;
            stg->argv[argc] = NULL; // Required by system call
            for (int i = 0; i < argc; ++i) {
                stg->argv[i] = dequePopFront(argStack);
            }

            dequePushBack(ss, stg);
            stg = (stage*)malloc(sizeof(stage));
            stg->argv = NULL;
            stg->stdinArg = stg->stdoutArg = NULL;
            stg->stdin = STDIN_PIPED;
            stg->stdout = STDOUT_NORMAL;
        } else {
            dequePushBack(argStack, tok);
        }
    }
    if (DEQUE_ISEMPTY(argStack)) {
        dequeFreeAll(ss); deleteDeque(ss);
        dequeFreeAll(argStack); deleteDeque(argStack);
        RAISE(EXCEPTION_PASER_MISSING_EXECUTABLE, NULL);
    }
    int argc = argStack->count;
    stg->argv = (char**) malloc(sizeof(char*) * argc);
    stg->argc = argStack->count;
    stg->argv[argc] = NULL; // Required by system call
    for (int i = 0; i < argc; ++i) {
        stg->argv[i] = dequePopFront(argStack);
    }

    dequePushBack(ss, stg);

    dequeFreeAll(argStack); deleteDeque(argStack);

    return ss;
}