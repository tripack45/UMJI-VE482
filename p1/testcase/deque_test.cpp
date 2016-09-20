#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../src/deque.h"
}

TEST(DEQUE, TC1SUCCESS) {
    deque *dq = newDeque(5);
    const char* str[] = {"0abc", "1def", "2efg", "3hij", "", "a", " "};
    dequePushBack(dq, str[0]); //0
    dequePushBack(dq, str[2]); //0 2
    const char* s1 = (char*) dequePopFront(dq); // 2
    dequePushBack(dq, str[3]); // 2 3
    dequePushFront(dq, str[5]); // 5 2 3
    const char* s2 = (char*) dequePopBack(dq); // 5 2
    dequePushFront(dq, str[5]); // 5 5 2
    const char* s3 = (char*) dequePopFront(dq); // 5 2
    dequePushFront(dq, str[4]); // 4 5 2
    const char* s4 = (char*) dequePopFront(dq); // 5 2
    const char* s5 = (char*) dequePopBack(dq); // 5
    const char* s6 = (char*) dequePopFront(dq); //
    ASSERT_STREQ(s1, str[0]);
    ASSERT_STREQ(s2, str[3]);
    ASSERT_STREQ(s3, str[5]);
    ASSERT_STREQ(s4, str[4]);
    ASSERT_STREQ(s5, str[2]);
    ASSERT_STREQ(s6, str[5]);
}