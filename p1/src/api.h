//
// Created by tripack on 16-9-20.
//

#ifndef VE482_API_H
#define VE482_API_H

#include "parser.h"

// ====== POSIX_HEADER ======

typedef struct {
    int stdoutFd;
    int stdErrFd;
} context;

typedef int (*builtinFun)(int,const char**);

extern context ctx;

void initializeContext(context *ctx);

const char* pwd();

void executeExtern(stage* stg, context *ctx);

void executeBuiltIn(stage* stg, context *ctx, builtinFun p);

void cd(const char* newDir);

int getRunningPid();

void actionExit(int signum);

void attachSigInt(void (*fun)(int));

void attachSigChd(void (*fun)(int));

void actionSigChd(int signum);

#endif //VE482_API_H
