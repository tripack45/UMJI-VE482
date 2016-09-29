//
// Created by tripack on 16-9-20.
//

#ifndef VE482_EXECUTE_H
#define VE482_EXECUTE_H

#include "parser.h"
#include "api.h"

#define COMMAND_BUILTIN_EXIT    0xc1
#define COMMAND_BUILTIN_CD      0xc2
#define COMMAND_BUILTIN_PWD     0xc3
#define COMMAND_EXTERNAL        0xc4

int identifyBuiltin(char* cmd);

int execute(stageStack *ss, context *ctx);

int exeCd(int argc, char* argv[]);

int exePwd(int argc, char* argv[]);

int exeDummy(int argc, char* argv[]);

#endif //VE482_EXECUTE_H
