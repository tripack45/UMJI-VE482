//
// Created by tripack on 16-9-18.
//

#ifndef VE482_DEQUE_H
#define VE482_DEQUE_H

typedef const void* dataptr;

typedef struct {
    int size;
    int count;
    dataptr *data;
    int head;
    int tail;
}deque;

deque *newDeque(int size);

#define DEQUE_ISEMPTY(deque) (deque->count == 0)
#define DEQUE_ISFULL(deque) (deque->size == deque->count)

void dequePushBack(deque* dq,const void* elem);
void dequePushFront(deque* dq,const void* elem);
void* dequePopBack(deque* dq);
void* dequePopFront(deque* dq);

void dequePopAll(deque* dq);
void dequeFreeAll(deque* dq);

void deleteDeque(deque* dq);


#endif //VE482_DEQUE_H
