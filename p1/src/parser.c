//
// Created by tripack on 16-9-19.
//

#include "parser.h"
#include "exception.h"
#include "assert.h"
#include "deque.h"
#include "tokenizer.h"
#include "oop.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// parse the given token stream
// append the results into stage stream
// Require both arguments are none - null
// TODO: change the token syntax into clone()
void parse(tokenStack *ts, stageStack *ss) {
    assert(ts != NULL);
    assert(ss != NULL);

    if (ts->isEmpty(ts)) return;
    stage *stg = NULL;

    stg =  NEW(stage)();
    token *tok = NULL;

    int e = 0;
    while (!ts->isEmpty(ts)) {
        tok = ts->popFront(ts);
        // Sanity check: if there exists more than 1 redirect
        if (tok->type == TOKEN_STRING) {
            stg->argStack->pushBack(stg->argStack,
                                    tok->cloneContent(tok));
            tok->del(tok);
            continue;
        }

        if (tok->type == TOKEN_REDIR_STDIN) {
            if (stg->stdin != STDIN_NORMAL) {
                ts->pushFront(ts, tok);
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_TOO_MANY_STDIN_REDIR);
            }
            if (ts->isEmpty(ts)) {
                ts->pushFront(ts, tok);
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_MISSING_REDIRECTION_FILE);
            }
            token *fileToken = ts->popFront(ts);
            if (isOperator(fileToken->type)) {
                ts->pushFront(ts, fileToken);
                ts->pushFront(ts, tok);
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_MISSING_REDIRECTION_FILE);
            }
            stg->stdin = STDIN_FILE;
            stg->stdinArg = fileToken->cloneContent(fileToken);
            // Mimics the move sematic
            // So that the content will not be freed on deletion

            // Consumes these arguments
            tok->del(tok);
            fileToken->del(tok);
            continue; // Next Token
        }

        if (tok->type == TOKEN_REDIR_STDOUT_TRUNC ||
                tok->type == TOKEN_REDIR_STDOUT_APPEND) {

            int newMode = tok->type == TOKEN_REDIR_STDOUT_APPEND ?
                          STDOUT_FILE_APPEND:
                          STDOUT_FILE_TRUNCATE;

            if (stg->stdout != STDOUT_NORMAL) {
                ts->pushFront(ts, tok);
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR);
            }
            if (ts->isEmpty(ts)) {
                ts->pushBack(ts, tok);
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_MISSING_REDIRECTION_FILE);
            }
            token *fileToken = ts->popFront(ts);
            if (isOperator(fileToken->type)) {
                ts->pushBack(ts, fileToken);
                ts->pushBack(ts, tok);
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_MISSING_REDIRECTION_FILE);
            }
            stg->stdout = newMode;
            stg->stdoutArg = fileToken->cloneContent(fileToken);

            tok->del(tok);
            fileToken->del(fileToken);

            continue;
        }

        if (tok->type == TOKEN_PIPE) {
            if (stg->stdin != STDIN_NORMAL || stg->stdout != STDOUT_NORMAL) {
                ts->pushFront(ts, tok);
                int inMode = stg->stdin;
                stg->del(stg);
                if (inMode != STDIN_NORMAL) {
                    RAISE_VOID(EXCEPTION_PASER_TOO_MANY_STDIN_REDIR);
                } else {
                    RAISE_VOID(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR);
                }
            }
            stg->stdout = STDOUT_PIPED;
            tok->del(tok);

            if (stg->argStack->isEmpty(stg->argStack)) {
                stg->del(stg);
                RAISE_VOID(EXCEPTION_PASER_MISSING_EXECUTABLE);
            }

            ss->pushBack(ss, stg);
            // This stage is over here

            // Setup a new stage
            stg = NEW(stage)();
            stg->stdin = STDIN_PIPED;
            continue;
        }
    }

    if (stg->argStack->isEmpty(stg->argStack)) {
        stg->del(stg);
        RAISE_VOID(EXCEPTION_PASER_MISSING_EXECUTABLE);
    }

    ss->pushBack(ss, stg);
}

stringStack *new_stringStack() {
    return NEW(deque)();
}

stage *new_stage() {
    stage *ss = malloc(sizeof(stage));
    ss->del = stageDelete;
    ss->argStack = NEW(stringStack)();
    ss->stdin = STDIN_NORMAL;
    ss->stdout = STDOUT_NORMAL;
    ss->stdinArg = ss->stdoutArg = NULL;
}

void stageDelete(stage *obj) {
    obj->argStack->del(obj->argStack);
    if (obj->stdinArg != NULL) free(obj->stdinArg);
    if (obj->stdoutArg != NULL) free(obj->stdoutArg);
    free(obj);
}

stageStack *new_stageStack() {
    stageStack *ss = NEW(deque)();
    ss->clear = stageStackClear;
}

void stageStackClear(stageStack *obj) {
    while(!obj->isEmpty(obj)) {
        stage *s = obj->popFront(obj);
        s->del(s);
    }
    free(obj);
}