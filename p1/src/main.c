//
// Created by patri on 2016/9/16.
//
//
// /home/tripack/.CLion2016.2/system/cmake/generated/UMJI-VE482-3d8a514c/3d8a514c/Debug/p1

#include "tokenizer.h"
#include "parser.h"
#include "exception.h"
#include "signal_action.h"

#include <stdio.h>
#include <stdlib.h>

//====== POSIX HEADERS

char *buffer = NULL;
const int bufferSize = 1200;

void freeBuffer();
void handleCmd(char* cmd);

int main() {
    atexit(freeBuffer);
    buffer = malloc(sizeof(char) * bufferSize);
    attachSigInt(actionExit);

    for (;;) {
        printf("user@ve482: $ ");
        fgets(buffer, bufferSize, stdin);
        handleCmd(buffer);

    }
}

void freeBuffer() {
    free(buffer);
}

void handleCmd(char* buffer) {
    tokenStack *ts = tokenize(buffer);
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
        printf("Unknown error, error code [%d]\n", errcode());
        resetError();
        return;
    }
    stageStack *ss = parse(ts);
    if(errcode()) {
        dequeFreeAll(ts);
        deleteDeque(ts);
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
        printf("Syntax Error: Unknown error, error code [%d]\n", errcode());
        resetError();
        return;
    }
    printf("command successfully parsed!\n");
}


