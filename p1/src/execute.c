//
// Created by tripack on 16-9-20.
//

#include "execute.h"
#include "exception.h"
#include "api.h"
#include "parser.h"
#include "oop.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exeCd(int argc, char* argv[]) {
    if (argc > 2) {
        puts("cd : Too many arguments");
        return -1;
    }
    if (argc < 2) { // Just "cd", we do nothing
        return 0;
    }
    cd(argv[1]);
    CATCH(EXCEPTION_RUNTIME_PERMISSION_DENIED) {
        puts("cd : Runtime Error, permission denied");
        resetError();
    } CATCH(EXCEPTION_RUNTIME_PATH_TOO_LONG) {
        puts("cd : Runtime Error, argument path too long");
        resetError();
    } CATCH(EXCEPTION_RUNTIME_PATH_NOT_EXIST) {
        puts("cd : Runtime Error, path does not exist");
        resetError();
    } CATCH(EXCEPTION_UNKNOWN) {
        puts("cd : Unknow Error.");
        resetError();
    }
    return 0;
}

int exePwd(int argc, char* argv[]) {
    if (argc > 1) {
        puts("pwd : Too many arguments");
        return -1;
    }
    const char* path = pwd();
    CATCH(EXCEPTION_RUNTIME_PATH_TOO_LONG) {
        puts("pwd : Current path too long!");
        resetError();
        return -1;
    }
    puts(path);
    return 0;
}

int exeDummy(int argc, char* argv[]) {
    // A dummy programming, act as an place holder
    return 0;
}

int identifyBuiltin(char* cmd) {
    if (!strcmp(cmd, "exit")) return COMMAND_BUILTIN_EXIT;
    if (!strcmp(cmd, "cd")) return COMMAND_BUILTIN_CD;
    if (!strcmp(cmd, "pwd")) return COMMAND_BUILTIN_PWD;
    return COMMAND_EXTERNAL;
}

int execute(stageStack *ss, context *ctx) {
    processInfo *thisInfo = NEW(processInfo)();
    processInfo *nextInfo = NEW(processInfo)();
    while (!ss->isEmpty(ss)) {
        stage *s = ss->popFront(ss);
        setupIO(s, thisInfo, nextInfo);
        if(errcode()) resetError(), perror("");
        stringStack *argStack = s->argStack;
        char* executable = argStack->front(argStack);
        int cmd_type = identifyBuiltin(executable);
        if (cmd_type != COMMAND_EXTERNAL) {
            switch (cmd_type) {
                case COMMAND_BUILTIN_EXIT:
                    exit(0);
                case COMMAND_BUILTIN_CD: {
                    int count = argStack->count;
                    char **argv = (char**)argStack->cloneToArray(argStack);
                    exeCd(count, argv);
                    executeBuiltIn(s, ctx, thisInfo, exeDummy);
                    freeArray((void*)argv);
                    break;
                }
                case COMMAND_BUILTIN_PWD:
                    executeBuiltIn(s, ctx, thisInfo, exePwd);
                    break;
                default:
                    executeBuiltIn(s, ctx, thisInfo, exeDummy);
            }
        } else {
            executeExtern(s, ctx, thisInfo);
        }
        ctx->regist(ctx, thisInfo);
        thisInfo = nextInfo;
        nextInfo = NEW(processInfo)();
        s->del(s);
    }
    thisInfo->del(thisInfo);
    nextInfo->del(nextInfo);

    return 0;
}
