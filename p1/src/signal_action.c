//
// Created by tripack on 16-9-20.
//

#include "signal_action.h"
#include <stdlib.h>

// ======= POSIX_HEADER ========
#include <signal.h>

struct sigaction sigIntAction;
struct sigaction prevIntAction;

void attachSigInt(void (*fun)(int)) {
    sigIntAction.sa_handler = fun;
    sigaction(SIGINT, &sigIntAction, &prevIntAction);
}

void actionExit(int signum) {
    exit(0);
}

