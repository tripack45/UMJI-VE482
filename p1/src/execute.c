//
// Created by tripack on 16-9-20.
//

#include "execute.h"
#include "exception.h"
#include "api.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exeCd(int argc, const char* argv[]) {
    if (argc > 2) {
        puts("cd : Too many arguments");
        return -1;
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
}

int exePwd(int argc, const char* argv[]) {
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
}

int exeDummy(int argc, const char* argv[]) {
    // A dummy programming, act as an place holder
}


int execute(stageStack *ss) {
    initializeContext(&ctx);
    while (!ss->isEmpty(ss)) {
        stage *s = ss->popFront(ss);
        char* executable = s->argv[0];
        if (strcmp(executable, "exit") == 0) {
            exit(0);
        } else if (strcmp(executable, "cd") == 0) {
            int count = 0;
            while (s->argv[++count] != NULL);
            exeCd(count, (const char **)(s->argv));
            executeBuiltIn(s, &ctx, exeDummy);
        } else if (strcmp(executable, "pwd") == 0) {
            executeBuiltIn(s, &ctx, exePwd);
        } else {
            executeExtern(s, &ctx);
        }
        free(s);
    }
    ss->del(ss);
}
