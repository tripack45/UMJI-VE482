//
// Created by tripack on 16-9-18.
//

#define DEQUE_EXPOSE_PRIVATE

#include "deque.h"
#include "exception.h"

#include <stdlib.h>
#include <assert.h>

deque *new_deque() {
    deque *d = malloc(sizeof(deque));

    d->isEmpty= dequeIsEmpty;

    d->pushBack = dequePushBack;
    d->popBack = dequePopBack;
    d->pushFront = dequePushFront;
    d->popFront = dequePopFront;

    d->front = dequeFront;
    d->back = dequeBack;

    d->clear = dequeClear;
    d->del = dequeDelete;

    d->count = 0;

    d->head.prev = d->tail.next = NULL;
    d->head.next = &(d->tail);
    d->tail.prev = &(d->head);
}

int dequeIsEmpty(deque *obj) {
    return obj->count == 0;
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

dataptr deleteNode(deque* obj, node* victim) {
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
    return deleteNode(obj, victim);
}

dataptr dequePopFront(deque* obj) {
    if (obj->isEmpty(obj))
        RAISE(EXCEPTION_DEQUE_EMPTY, NULL);

    node *victim = obj->head.next;
    return deleteNode(obj, victim);
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