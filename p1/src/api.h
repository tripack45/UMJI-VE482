//
// Created by tripack on 16-9-20.
//

#ifndef VE482_API_H
#define VE482_API_H

#include "parser.h"

// ====== POSIX_HEADER ======

typedef int (*builtinFun)(int, char**);

#define PROCESS_STATE_ERROR     0xB0
#define PROCESS_STATE_READY     0xB1
#define PROCESS_STATE_RUNNING   0xB2
#define PROCESS_STATE_DONE      0xB3

typedef struct processInfo_t{
    int pid;
    int state;
    int stdinFd;
    int stdoutFd;

    void (*del)(struct processInfo_t *obj);
} processInfo;
processInfo *new_processInfo();
#ifdef API_EXPOSE_PRIVATE
void processInfoDelete(processInfo *obj);
#endif

typedef deque pInfoList;
pInfoList *new_pInfoList();

typedef struct context_t{
    pInfoList *infoList;

    void (*regist)(struct context_t *obj, processInfo *info);
    void (*waitAll)(struct context_t *obj);
    //void (*killById)(struct context_t *obj, int pid);
    void (*killAll)(struct context_t *obj);
    void (*del)(struct context_t *obj);
} context;
context *new_context();
#ifdef API_EXPOSE_PRIVATE
void contextRegist(context *ctx, processInfo *info);
void contextWaitAll(context *ctx);
//void contextKillById(context *ctx, int pid);
void contextKillAll(context *ctx);
void contextDelete(context* ctx);
#endif

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

void attachSignal(int signal, void (*fun)(int));

void attachSigint(void (*fun)(int));

int translateError(int errcode);

context *getContext();
void setContext(context *c);



#endif //VE482_API_H
