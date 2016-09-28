//
// Created by tripack on 16-9-18.
//

#ifndef VE482_DEQUE_H
#define VE482_DEQUE_H

// Object oriented is the best

typedef void* dataptr;

typedef struct node_t{
    struct node_t *prev;
    struct node_t *next;
    dataptr value;
} node;

typedef struct deque_t{
    int count;
    node head; // Node instead of pointer for regualrity
    node tail;

    int (*isEmpty)(struct deque_t *obj);

    void (*pushBack)(struct deque_t *obj,
                     const dataptr elem);
    void (*pushFront)(struct deque_t *obj,
                      const dataptr elem);
    dataptr (*popBack)(struct deque_t *obj);
    dataptr (*popFront)(struct deque_t *obj);

    dataptr (*front)(struct deque_t *obj);
    dataptr (*back)(struct deque_t *obj);

    void (*clear)(struct deque_t *obj);
    void (*del)(struct deque_t *obj);
} deque;

deque* new_deque();

#ifdef DEQUE_EXPOSE_PRIVATE

int dequeIsEmpty(deque* obj);

void dequePushBack(deque* obj, const dataptr elem);
void dequePushFront(deque* obj,const dataptr elem);
dataptr dequePopBack(deque* obj);
dataptr dequePopFront(deque* obj);

dataptr dequeFront(deque* obj);
dataptr dequeBack(deque* obj);

void dequeClear(deque* obj);

void dequeDelete(deque* obj);

dataptr deleteNode(deque* obj, node* victim);

#endif

#endif //VE482_DEQUE_H
