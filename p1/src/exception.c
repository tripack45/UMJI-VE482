//
// Created by tripack on 16-9-18.
//

#include "exception.h"


int lastError;

void setError(int e) {
    lastError = e;
}

void resetError() {
    lastError = 0;
}

int errcode() {
    return lastError;
}
