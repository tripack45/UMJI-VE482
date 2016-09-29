//
// Created by tripack on 16-9-20.
//

#ifndef VE482_API_H
#define VE482_API_H

#include "parser.h"

// ====== POSIX_HEADER ======

typedef int (*builtinFun)(int, char**);

typedef struct processInfo_t{
    int pid;
    int stdinFd;
    int stdoutFd;

    void (*del)(struct processInfo_t *obj);
} processInfo;
processInfo *new_processInfo();

typedef struct context_t{
    deque *infoList;

    void (*regist)(processInfo *obj);
    void (*waitAll)(struct context_t *obj);
    void (*killById)(struct context_t *obj, int pid);
    void (*killAll)(struct context_t *obj);
    void (*del)(struct context_t *obj);
} context;
context *new_context();

void executeExtern(stage* stg, // Stage to execute
                   context *ctx, // Context that its executing
                   processInfo *info); // Store into p_info
void executeBuiltIn(stage* stg,
                    context *ctx,
                    processInfo *info,
                    builtinFun p);

void setupIO(stage *s,
             processInfo *thisInfo,
             processInfo *nextInfo);

const char* pwd();
void cd(const char* newDir);

int getRunningPid();

void actionExit(int signum);

void attachSigInt(void (*fun)(int));
void attachSigChd(void (*fun)(int));
void actionSigChd(int signum);

int translateError(int errcode);

#endif //VE482_API_H
