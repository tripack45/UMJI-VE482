//
// Created by tripack on 16-9-20.
//
#define API_EXPOSE_PRIVATE

#include "api.h"
#include "exception.h"
#include "parser.h"
#include "oop.h"

#include <stdlib.h>
#include <stdio.h>

// This is the OS Abstraction Layer
// We deal with all OS Sensitive stuff here

// ============ POSIX_HEADER ============
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>



const char* pwd() {
    static char buf[512] = {0};
    char* ret = getcwd(buf, 512);
    if (ret == NULL) {
        int e = errno;
        RAISE(translateError(e), NULL);
    }
    return buf;
}

void cd(const char *newDir) {
    int ret = chdir(newDir);
    if (ret == -1) {
        int e = errno;
        RAISE_VOID(translateError(e));
    }
}

void setupIO(stage* stg, processInfo *thisInfo, processInfo *nextInfo)  {
    int e = 0;
    switch (stg->stdin) {
        case STDIN_FILE: {
            int fd = open(stg->stdinArg, O_RDONLY);
            if (fd == -1) { // Open is unsucessful
                thisInfo->state = PROCESS_STATE_ERROR;
                e = errno;
                RAISE_VOID(translateError(e));
            }
            thisInfo->stdinFd = fd;
            break;
        }
        case STDIN_PIPED: {
            // Allow current pipe to be passed through fork() and exec()
            int currFlag = fcntl(thisInfo->stdinFd, F_GETFD);
            fcntl(thisInfo->stdinFd, F_SETFD, currFlag & ~O_CLOEXEC);
            break;
        }
        case STDIN_NORMAL:
        default:;
            // These cases don't need to open files
    }
    switch (stg->stdout) {
        case STDOUT_FILE_TRUNCATE:
        case STDOUT_FILE_APPEND: {
            unsigned int baseOptions = O_WRONLY | O_CREAT;
            unsigned int mode = S_IRWXU | S_IRWXG | S_IROTH;
            int outFd = stg->stdout == STDOUT_FILE_TRUNCATE ?
                     open(stg->stdoutArg, baseOptions | O_TRUNC, mode) :
                     open(stg->stdoutArg, baseOptions | O_APPEND, mode);
            if (outFd == -1) {
                thisInfo->state = PROCESS_STATE_ERROR;
                e = errno;
                RAISE_VOID(translateError(e));
            }
            break;
        }
        case STDOUT_PIPED: {
            int pipeFd[2] = {0};
            pipe(pipeFd);
            thisInfo->stdoutFd = pipeFd[1]; // Write into "Write End";
            nextInfo->stdinFd  = pipeFd[0]; // Puts into then read of next one
            // Disallow the future info be passed through fork() and exec()
            int currFlag = fcntl(thisInfo->stdinFd, F_GETFD);
            fcntl(nextInfo->stdinFd, F_SETFD, currFlag | O_CLOEXEC);

            //fprintf(stderr, "Piped to fd = %d\n", *outFd);
            //fprintf(stderr, "Pipe other end to fd = %d\n", pipeFd[0]);
            break;
        }
        case STDOUT_NORMAL:
        default:;
    }
}

void executeBuiltIn(stage* stg,
                    context *ctx,
                    processInfo *info,
                    builtinFun programme) {
    // TODO: Deal with processes failed when being setup
    stringStack *argStack = stg->argStack;
    char **argv = (char**)(argStack->cloneToArray(argStack));
    int argc = argStack->count;
    int pid = fork();
    if (pid) {
        info->pid = pid;
        // Now we releases these files in the parent process
        if (info->stdinFd > 0) close(info->stdinFd);
        if (info->stdoutFd > 0) close(info->stdoutFd);
        info->stdinFd = -1;
        info->stdoutFd = -1;
        freeArray((void*)argv);
    } else {
        // Child process
        if (info->stdinFd > 0)
            dup2(info->stdinFd, STDIN_FILENO);
        if (info->stdoutFd > 0)
            dup2(info->stdoutFd, STDOUT_FILENO);
        int ret = programme(argc, argv);
        exit(ret);
    }
}


void executeExtern(stage* stg,
                   context *ctx,
                   processInfo *info) {
    stringStack *argStack = stg->argStack;
    char **argv = (char**)(argStack->cloneToArray(argStack));
    int pid = fork();
    if (pid) {
        info->pid = pid;
        // Now we releases these files in the parent process
        if (info->stdinFd > 0) close(info->stdinFd);
        if (info->stdoutFd > 0) close(info->stdoutFd);
        info->stdinFd = -1;
        info->stdoutFd = -1;
    } else {
        // Child process
        if (info->stdinFd > 0)
            dup2(info->stdinFd, STDIN_FILENO);
        if (info->stdoutFd > 0)
            dup2(info->stdoutFd, STDOUT_FILENO);
        int ret = execvp(argv[0], argv);
        if (ret == -1) {
            fprintf(stderr, "Error when executing command.\n");
        }
        exit(ret);
    }
    freeArray((void*)argv);
}

struct sigaction sigIntAction;
struct sigaction prevIntAction;

struct sigaction sigChdAction;
struct sigaction prevChdAction;

void attachSigInt(void (*fun)(int)) {
    sigIntAction.sa_handler = fun;
    sigaction(SIGINT, &sigIntAction, &prevIntAction);
}

void attachSigChd(void (*fun)(int)) {
    sigChdAction.sa_handler = fun;
    sigaction(SIGCHLD, &sigChdAction, &prevChdAction);
}

void actionExit(int signum) {
    fprintf(stderr, "\n");
    // Todo: Rewrite this using "context"
}

void actionSigChd(int signum) {
    int pid;
    while( pid = waitpid(-1, NULL, WNOHANG) > 0) {
        fprintf(stderr, "Waiting %d \n", pid);
    }
}

context *new_context() {
    context *ctx = malloc(sizeof(ctx));

    ctx->regist = contextRegist;
    ctx->waitAll = contextWaitAll;
    //ctx->killById = contextKillById;
    ctx->killAll = contextKillAll;

    ctx->del = contextDelete;

    ctx->infoList = NEW(pInfoList)();
}

pInfoList *new_pInfoList() {
    pInfoList *pL = NEW(deque)();
    // No need to overrive clear()
    // We assume processInfo can be directly freed safely
}

void contextRegist(context *ctx, processInfo *info) {
    ctx->infoList->pushBack(ctx->infoList, info);
}

void contextWaitAll(context *ctx) {
    int pid = -1;
    pInfoList *infoList = ctx->infoList;
    while ((pid = wait(NULL)) > 0) {
        node *begin = infoList->head.next;
        node *end = &infoList->tail;
        for (node *n = begin; n != end; n = n->next) {
            processInfo *pInfo = n->value;
            if (pInfo->pid == pid) {
                infoList->deleteNode(infoList, n);
                pInfo->del(pInfo);
            }
        }
        assert(1); // We have a leak. A process is started but not managed
    }
}

//void contextKillById(context *ctx, int pid);

void contextKillAll(context *ctx) {
    deque *infoList = ctx->infoList;
    node *begin = infoList->head.next;
    node *end = &infoList->tail;
    for (node *n = begin; n != end; n = n->next) {
        processInfo *pInfo = n->value;
        kill(pInfo->pid, SIGTERM);
    }
}

void contextDelete(context* ctx) {
    ctx->infoList->del(ctx->infoList);
    free(ctx);
}

processInfo *new_processInfo() {
    processInfo *pInfo = malloc(sizeof(processInfo));

    pInfo->del = processInfoDelete;

    pInfo->state = PROCESS_STATE_READY;
    pInfo->pid = -1;
    pInfo->stdinFd = -1;
    pInfo->stdoutFd = -1;

    // These default values are platform specific.
    // In linux all three values will not be negative
    // if they are valid.

    return pInfo;
}

void processInfoDelete(processInfo *obj) {
    free(obj);
    // TODO: if file fields are none-negative, we have a leak.
    assert(obj->stdinFd > 0);
    assert(obj->stdoutFd > 0);
}

int translateError(int e) {
    switch (e) {
        case EACCES:        return EXCEPTION_RUNTIME_PERMISSION_DENIED;
        case ENAMETOOLONG:  return EXCEPTION_RUNTIME_PATH_TOO_LONG;
        case EISDIR:        return EXCEPTION_RUNTIME_NOT_A_FILE;
        case ENOENT:        return EXCEPTION_RUNTIME_FILE_NOT_EXIST;
        case ERANGE:        return EXCEPTION_RUNTIME_PATH_TOO_LONG;
        default:            return EXCEPTION_UNKNOWN;
    }
}