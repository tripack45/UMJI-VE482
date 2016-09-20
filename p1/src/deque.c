//
// Created by tripack on 16-9-18.
//
#include "deque.h"
#include "exception.h"

#include <stdlib.h>
#include <assert.h>

deque *newDeque(int size) {
    if (size < 0)
        RAISE(EXCEPTION_DEQUE_ILLEGAL_CTOR, NULL);
    deque* dq = (deque*)malloc(sizeof(deque));
    dq->data = malloc(sizeof(void*) * size);
    dq->size = size;
    dq->count = 0;
    dq->head = 0;
    dq->tail = 1;
    return dq;
}

void dequePushBack(deque* dq, const void* elem) {
    assert(dq != NULL);
    if (DEQUE_ISFULL(dq))
        RAISE_VOID(EXCEPTION_DEQUE_FULL);
    dq->data[dq->tail] = elem;
    dq->tail = (dq->tail + 1) % dq->size;
    dq->count++;
}

void dequePushFront(deque* dq, const void* elem) {
    assert(dq != NULL);
    if (DEQUE_ISFULL(dq))
        RAISE_VOID(EXCEPTION_DEQUE_FULL);
    dq->data[dq->head] = elem;
    dq->head = (dq->head + dq->size -1) % dq->size;
    dq->count++;
}

void* dequePopBack(deque* dq) {
    assert(dq != NULL);
    if (DEQUE_ISEMPTY(dq))
        RAISE(EXCEPTION_DEQUE_EMPTY, NULL);
    dq->tail = (dq->tail + dq->size -1) % dq->size;
    dq->count --;
    dataptr *ret = dq->data[dq->tail];
    return ret;
}

void* dequePopFront(deque* dq) {
    assert(dq != NULL);
    if (DEQUE_ISEMPTY(dq))
        RAISE(EXCEPTION_DEQUE_EMPTY, NULL);
    dq->head = (dq->head + 1) % dq->size;
    dq->count --;
    dataptr *ret = dq->data[dq->head];
    return ret;
}

void dequePopAll(deque* dq) {
    assert(dq != NULL);
    while (!DEQUE_ISEMPTY(dq))
        dequePopBack(dq);
}

void dequeFreeAll(deque* dq) {
    assert(dq != NULL);
    while (!DEQUE_ISEMPTY(dq))
        free(dequePopBack(dq));
}

void deleteDeque(deque* dq) {
    assert(dq != NULL);
    if (!DEQUE_ISEMPTY(dq))
        RAISE_VOID(EXCEPTION_DEQUE_NONE_EMPTY_DTOR);
    free(dq->data);
    free(dq);
}