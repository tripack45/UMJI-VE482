//
// Created by patri on 2016/9/16.
//
//
// /home/tripack/.CLion2016.2/system/cmake/generated/UMJI-VE482-3d8a514c/3d8a514c/Debug/p1

#include "tokenizer.h"
#include "parser.h"
#include "exception.h"
#include "api.h"
#include "execute.h"
#include "oop.h"

#include <stdio.h>
#include <stdlib.h>

//====== POSIX HEADERS

char *buffer = NULL;
const int bufferSize = 1200;

void freeBuffer();
void handleCmd(char* cmd);

void sigintHandler(int signum);

#ifdef __linux__
const char *platform = "ve482@linux";
#else
const char *platform = "ve482@minix";
#endif

int main() {
    atexit(freeBuffer);
    buffer = malloc(sizeof(char) * bufferSize);
    attachSigint(sigintHandler);
    //attachSigInt(actionExit);
    //attachSigChd(actionSigChd);

    for (;;) {
        printf("%s:%s $ ",platform, pwd());
        char term = readRaw(buffer, bufferSize);
        if (term <= '\0') {
            // We have an special ending situation
            if (buffer[0] == '\0' && feof(stdin)) {
                // Nothing but CTRL+D is inputed.
                exit(0);
            }
        } else {
            handleCmd(buffer);
        }
        if (term != '\n' && term != '\r')
            printf("\n"); // Put an extra enter
    }
}

void freeBuffer() {
    free(buffer);
    //ctx->del(ctx);
}

void handleCmd(char* buffer) {
    context *ctx = NEW(context)();
    tokenStack *ts = NEW(tokenStack)();
    setContext(ctx);
    for (;;) {
        tokenize(buffer, ts);
        if (errcode()) ts->del(ts), ctx->del(ctx);
        CATCH(EXCEPTION_TOKENIZER_TOO_MANY_CHAR) {
            printf("Input Error: Too many characters, maximum 1024 characters!\n");
            resetError();
            return;
        }
        CATCH(EXCEPTION_TOKENIZER_UNBALANCED_QUOTE) {
            printf("Syntax Error: Unbalanced Quotation mark!\n");
            resetError();
            return;
        }
        CATCH_ELSE() {
            printf("Tokenizer: Unknown error, error code [%d]\n", errcode());
            resetError();
            return;
        }
        if (ts->isEmpty(ts)) break;
        token *lastToken = ts->back(ts);
        if (lastToken->type == TOKEN_STRING) break;
        // Wait for further input if last input is special operator
        printf("... ");
        readRaw(buffer, bufferSize);
    }
    stageStack *ss = NEW(stageStack)();
    parse(ts, ss);
    if(errcode()) {
        ts->del(ts);
        ss->del(ss);
        ctx->del(ctx);
    }
    CATCH(EXCEPTION_PASER_MISSING_EXECUTABLE) {
        printf("Syntax Error: Missing command!\n");
        resetError();
        return;
    }
    CATCH(EXCEPTION_PASER_MISSING_REDIRECTION_FILE) {
        printf("Syntax Error: Missing Redirecetion File!\n");
        resetError();
        return;
    }
    CATCH(EXCEPTION_PASER_REDIRECTION_PIPE_CONFLICT) {
        printf("Syntax Error: Redirecting IO while using pipes!\n");
        resetError();
        return;
    }
    CATCH(EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR) {
        printf("Syntax Error: Multiple Redirection on stdout!\n");
        resetError();
        return;
    }
    CATCH(EXCEPTION_PASER_TOO_MANY_STDIN_REDIR) {
        printf("Syntax Error: Multiple Redirection on stdin!\n");
        resetError();
        return;
    }
    CATCH_ELSE() {
        printf("parser: Unknown error, error code [%d]\n", errcode());
        resetError();
        return;
    }
    execute(ss, ctx);
    ctx->waitAll(ctx);

    ts->del(ts);
    ss->del(ss);
    setContext(NULL);
    ctx->del(ctx);
}

void sigintHandler(int signum) {
    context *ctx = getContext();
    if (ctx != NULL) {
        ctx->killAll(ctx);
        printf("\nUser Terminated");
    }
}


