//
// Created by tripack on 16-9-18.
//

#define DEQUE_EXPOSE_PRIVATE

#include "deque.h"
#include "exception.h"

#include <stdlib.h>

deque *new_deque() {
    deque *d = malloc(sizeof(deque));

    d->isEmpty= dequeIsEmpty;

    d->pushBack = dequePushBack;
    d->popBack = dequePopBack;
    d->pushFront = dequePushFront;
    d->popFront = dequePopFront;

    d->front = dequeFront;
    d->back = dequeBack;

    d->deleteNode = dequeDeleteNode;

    d->clear = dequeClear;
    d->del = dequeDelete;

    d->cloneToArray = deque2Array;

    d->count = 0;

    d->head.prev = d->tail.next = NULL;
    d->head.next = &(d->tail);
    d->tail.prev = &(d->head);
}

int dequeIsEmpty(deque *obj) {
    return obj->count == 0;
}

// Return an NULL terminated array of the content
// It works with the implementation, other then the abstraction
// It returns a "view", where content are not actually copied.
// It's a "shallow copy"
dataptr *deque2Array(deque *dq) {
    int count = dq->count;
    dataptr *array = calloc(sizeof(dataptr), count + 1);
    node *begin = dq->head.next;
    node *end = &dq->tail;
    int loc = 0;
    for (node *n = begin; n != end; n = n->next) {
        array[loc++] = n->value;
    }
    // loc points to one pass the end
    array[loc] = NULL;
    return array;
}

// Since content are not compied
// Only need release the array itself.
void freeArray(dataptr *array) {
    free(array);
}

void dequePushFront(deque* obj, const dataptr elem) {
    node* nextNode = obj->head.next;

    node* newNode = malloc(sizeof(node));
    newNode->prev = &(obj->head);
    newNode->next = nextNode;
    newNode->value = elem;

    obj->head.next = newNode;

    nextNode->prev = newNode;

    obj->count++;
}

void dequePushBack(deque* obj, const dataptr elem) {
    node* prevNode = obj->tail.prev;

    node* newNode = malloc(sizeof(node));
    newNode->prev = prevNode;
    newNode->next = &(obj->tail);
    newNode->value = elem;

    obj->tail.prev = newNode;

    prevNode->next = newNode;

    obj->count++;
}

dataptr dequeDeleteNode(deque* obj, node* victim) {
    dataptr ret = victim->value;

    node *next = victim->next;
    node *prev = victim->prev;

    prev->next = next;
    next->prev = prev;

    free(victim);

    obj->count--;

    return ret;
}

dataptr dequePopBack(deque* obj) {
    if (obj->isEmpty(obj))
        RAISE(EXCEPTION_DEQUE_EMPTY, NULL);

    node *victim = obj->tail.prev;
    return dequeDeleteNode(obj, victim);
}

dataptr dequePopFront(deque* obj) {
    if (obj->isEmpty(obj))
        RAISE(EXCEPTION_DEQUE_EMPTY, NULL);

    node *victim = obj->head.next;
    return dequeDeleteNode(obj, victim);
}

void dequeClear(deque* obj) {
    while(!obj->isEmpty(obj))
        free(obj->popFront(obj));
}

void dequeDelete(deque* obj) {
    if(!obj->isEmpty(obj))
        obj->clear(obj);
    free(obj);
}

dataptr dequeFront(deque *obj) {
    if (!obj->isEmpty(obj))
        return obj->head.next->value;
    RAISE(EXCEPTION_DEQUE_EMPTY, NULL);
}

dataptr dequeBack(deque *obj) {
    if (!obj->isEmpty(obj))
        return obj->tail.prev->value;
    RAISE(EXCEPTION_DEQUE_EMPTY, NULL);
}