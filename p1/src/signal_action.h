//
// Created by tripack on 16-9-20.
//

#ifndef VE482_SIGNAL_ACTION_H
#define VE482_SIGNAL_ACTION_H

void actionExit(int signum);

void attachSigInt(void (*fun)(int));

#endif //VE482_SIGNAL_ACTION_H
