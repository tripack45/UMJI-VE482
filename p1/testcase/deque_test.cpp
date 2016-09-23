#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../src/deque.h"
}

TEST(DEQUE, TC1SUCCESS) {
    deque *dq = NEW(deque)();
    const char* str[] = {"0abc", "1def", "2efg", "3hij", "", "a", " "};
    dq->pushBack(dq, (dataptr)(str[0])); //0
    dq->pushBack(dq, (dataptr)(str[2])); //0 2
    const char* s1 = (char*) dq->popFront(dq); // 2
    dq->pushBack(dq, (dataptr)(str[3])); // 2 3
    dq->pushFront(dq, (dataptr)(str[5])); // 5 2 3
    const char* s2 = (char*) dq->popBack(dq); // 5 2
    dq->pushFront(dq, (dataptr)(str[5])); // 5 5 2
    const char* s3 = (char*) dq->popFront(dq); // 5 2
    dq->pushFront(dq, (dataptr)(str[4])); // 4 5 2
    const char* s4 = (char*) dq->popFront(dq); // 5 2
    const char* s5 = (char*) dq->popBack(dq); // 5
    const char* s6 = (char*) dq->popFront(dq); //
    ASSERT_STREQ(s1, str[0]);
    ASSERT_STREQ(s2, str[3]);
    ASSERT_STREQ(s3, str[5]);
    ASSERT_STREQ(s4, str[4]);
    ASSERT_STREQ(s5, str[2]);
    ASSERT_STREQ(s6, str[5]);
}