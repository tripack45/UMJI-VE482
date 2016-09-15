#include <limits.h>
#include "gtest/gtest.h"

TEST(TC1, TC1SUCCESS) {
    ASSERT_EQ(1, 1);
}

TEST(TC1, TC1FAIL) {
    ASSERT_EQ(1, 2);
}

TEST(TC2, TC2SUCCESS) {
    ASSERT_EQ(1, 1);
}

TEST(TC2, TC2FAIL) {
    ASSERT_EQ(1, 2);
}

TEST(TC3, TC3SUCCESS) {
    ASSERT_EQ(1, 1);
}