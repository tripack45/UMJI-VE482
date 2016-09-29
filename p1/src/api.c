//
// Created by tripack on 16-9-20.
//
#include "api.h"
#include "exception.h"
#include "parser.h"

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

void setupIO(stage* stg, context *ctx, int *inFd, int *outFd) {
    *inFd = STDIN_FILENO;
    *outFd = STDOUT_FILENO;
    switch (stg->stdin) {
        case STDIN_NORMAL :
            *inFd = STDIN_FILENO;
            break;
        case STDIN_FILE:
            *inFd = open(stg->stdinArg, O_RDONLY);
            if (*inFd == -1) {
                int e = errno;
                RAISE_VOID(translateError(e)):
            }
            break;
        case STDIN_PIPED:
            *inFd = ctx->stdoutFd;
            //fprintf(stderr, "Piped from fd = %d\n", *inFd);
            break;
        default:
            break;
    }
    switch (stg->stdout) {
        case STDOUT_NORMAL:
            *outFd = STDOUT_FILENO;
            break;
        case STDOUT_FILE_TRUNCATE:
        case STDOUT_FILE_APPEND: {
            unsigned int baseOptions = O_WRONLY | O_CREAT;
            unsigned int mode = S_IRWXU | S_IRWXG | S_IROTH;
            *outFd = stg->stdout == STDOUT_FILE_TRUNCATE ?
                     open(stg->stdoutArg, baseOptions | O_TRUNC, mode) :
                     open(stg->stdoutArg, baseOptions | O_APPEND, mode);
            if (*outFd == -1) {
                int e = errno;

            }
            break;
        }
        case STDOUT_PIPED: {
            int pipeFd[2] = {0};
            pipe(pipeFd);
            *outFd = pipeFd[1]; // Write into "Write End";
            ctx->stdoutFd = pipeFd[0]; // wait for next program to read
            //fprintf(stderr, "Piped to fd = %d\n", *outFd);
            //fprintf(stderr, "Pipe other end to fd = %d\n", pipeFd[0]);
            break;
        }
    }
}

void executeBuiltIn(stage* stg, context *ctx, builtinFun programme) {
    int inFd; int outFd;
    setupIO(stg, ctx, &inFd, &outFd);
    CATCH_ELSE(){
        RAISE_VOID(LAST_EXCEPTION);
    }
    int pid = fork();
    if (pid) {
        // Parent process
        runningPid = pid;
        while (wait(NULL) != -1) ;
        runningPid = -1;
        // Wait until the child teminated
        if (stg->stdin == STDIN_FILE)
            close(inFd);
        if (stg->stdout == STDOUT_FILE_APPEND
            || stg->stdout == STDOUT_FILE_TRUNCATE)
            close(outFd);
        if (stg->stdin == STDIN_PIPED)
            close(ctx->stdoutFd);
    } else {
        // Child process
        dup2(inFd, STDIN_FILENO);
        dup2(outFd, STDOUT_FILENO);
        int count = 0;
        while (stg->argv[++count] != NULL);
        int ret = programme(count, (const char **)stg->argv);
        exit(0);
    }
}


void executeExtern(stage* stg, context *ctx) {
    int inFd; int outFd;
    setupIO(stg, ctx, &inFd, &outFd);
    CATCH_ELSE(){
        RAISE_VOID(LAST_EXCEPTION);
    }
    //fprintf(stderr, "inFd = %d, outFd = %d\n", inFd, outFd);
    int pid = fork();
    if (pid) {
        runningPid = pid;
        // Parent process
        //fprintf(stderr, "Waiting %s\n", stg->argv[0]);
        while (wait(NULL) != -1);
        // Wait until the child teminated
        if (stg->stdin != STDIN_NORMAL) {
            close(inFd);
            //fprintf(stderr, "Closed fd = %d", inFd);
        }
        if (stg->stdout != STDOUT_NORMAL) {
            close(outFd);
            //fprintf(stderr, "Closed fd = %d", outFd);
        }

    } else {
        // Child process
        dup2(inFd, STDIN_FILENO);
        dup2(outFd, STDOUT_FILENO);
        int ret = execvp(stg->argv[0], stg->argv);
        if (ret == -1) {
            //fprintf(stderr, "Error when executing command.\n");
        }
        exit(0);
    }
}

int getRunningPid() {
    return runningPid;
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
    int pid = getRunningPid();
    if (pid > 0) {
        fprintf(stderr, "\npid = %d\n", pid);
        kill(pid, SIGTERM);
        puts(" User terminated.");
    }
}

void actionSigChd(int signum) {
    int pid;
    while( pid = waitpid(-1, NULL, WNOHANG) > 0) {
        fprintf(stderr, "Waiting %d \n", pid);
    }
}

void initializeContext(context *ctx) {
    ctx->stdoutFd = STDOUT_FILENO;
    ctx->stdErrFd = STDERR_FILENO;
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