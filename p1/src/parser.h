//
// Created by tripack on 16-9-19.
//

#ifndef VE482_SEMANTIC_H
#define VE482_SEMANTIC_H

#include "deque.h"
#include "tokenizer.h"

#define STDIN_NORMAL 0x00F1
#define STDIN_PIPED 0x00F2
#define STDIN_FILE 0x00F3

#define STDOUT_NORMAL 0x00E1
#define STDOUT_PIPED 0x00E2
#define STDOUT_FILE_TRUNCATE 0x00E3
#define STDOUT_FILE_APPEND 0x00E4

//#define STDIO_UNSPECIFIED 0xCC


typedef struct {
    int argc;
    char* *argv;
    int stdin;
    char* stdinArg;
    int stdout;
    char* stdoutArg;
}stage;

typedef deque stageStack;

stageStack *parse(tokenStack *ts);

//void printStageStack(stageStack *ss);

#endif //VE482_SEMANTIC_H
