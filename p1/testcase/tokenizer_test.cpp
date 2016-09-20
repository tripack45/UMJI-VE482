//
// Created by tripack on 16-9-18.
//

#include <gtest/gtest.h>

extern "C" {
#include "../src/tokenizer.h"
}

TEST(TOKENIZER, SIMPLE) {
    const char *cmd = "ls";
    tokenStack *ts = tokenize(cmd);
    char *t0 = (char*)dequePopFront(ts);
    ASSERT_STREQ(t0, "ls");
    dequeFreeAll(ts);
    deleteDeque(ts);
}

TEST(TOKENIZER, BLANK) {
    const char *cmd0 = "";
    const char *cmd1 = " ";
    const char *cmd2 = "    ";
    const char *cmd3 = "    ";
    tokenStack *ts0 = tokenize(cmd0);
    tokenStack *ts1 = tokenize(cmd0);
    tokenStack *ts2 = tokenize(cmd0);
    tokenStack *ts3 = tokenize(cmd0);
    ASSERT_TRUE(DEQUE_ISEMPTY(ts0));
    ASSERT_TRUE(DEQUE_ISEMPTY(ts1));
    ASSERT_TRUE(DEQUE_ISEMPTY(ts2));
    ASSERT_TRUE(DEQUE_ISEMPTY(ts3));
    deleteDeque(ts0);
    deleteDeque(ts1);
    deleteDeque(ts2);
    deleteDeque(ts3);
}

TEST(TOKENIZER, MULTI) {
    const char *cmd = "  ls  -al    | grep abc | ls  -a  -b   >    123.txt  ";
    tokenStack *ts = tokenize(cmd);
    ASSERT_STREQ((char*)dequePopFront(ts), "ls");
    ASSERT_STREQ((char*)dequePopFront(ts), "-al");
    ASSERT_STREQ((char*)dequePopFront(ts), "|");
    ASSERT_STREQ((char*)dequePopFront(ts), "grep");
    ASSERT_STREQ((char*)dequePopFront(ts), "abc");
    ASSERT_STREQ((char*)dequePopFront(ts), "|");
    ASSERT_STREQ((char*)dequePopFront(ts), "ls");
    ASSERT_STREQ((char*)dequePopFront(ts), "-a");
    ASSERT_STREQ((char*)dequePopFront(ts), "-b");
    ASSERT_STREQ((char*)dequePopFront(ts), ">");
    ASSERT_STREQ((char*)dequePopFront(ts), "123.txt");
    ASSERT_TRUE(DEQUE_ISEMPTY(ts));
    dequeFreeAll(ts);
    deleteDeque(ts);
}

TEST(TOKENIZER, QUOTE) {
    const char *cmd = "  ls  -al \"| < a.txt\"   | grep abc | ls  -a  -b   >    123.txt  ";
    tokenStack *ts = tokenize(cmd);
    ASSERT_STREQ((char*)dequePopFront(ts), "ls");
    ASSERT_STREQ((char*)dequePopFront(ts), "-al");
    ASSERT_STREQ((char*)dequePopFront(ts), "| < a.txt");
    ASSERT_STREQ((char*)dequePopFront(ts), "|");
    ASSERT_STREQ((char*)dequePopFront(ts), "grep");
    ASSERT_STREQ((char*)dequePopFront(ts), "abc");
    ASSERT_STREQ((char*)dequePopFront(ts), "|");
    ASSERT_STREQ((char*)dequePopFront(ts), "ls");
    ASSERT_STREQ((char*)dequePopFront(ts), "-a");
    ASSERT_STREQ((char*)dequePopFront(ts), "-b");
    ASSERT_STREQ((char*)dequePopFront(ts), ">");
    ASSERT_STREQ((char*)dequePopFront(ts), "123.txt");
    ASSERT_TRUE(DEQUE_ISEMPTY(ts));
    dequeFreeAll(ts);
    deleteDeque(ts);
}


TEST(TOKENIZER, COMPLEX) {
    const char *cmd = "ps -aux | grep \"abc \" > a.txt";
    tokenStack *ts = tokenize(cmd);

    ASSERT_STREQ((char*)dequePopFront(ts), "ps");
    ASSERT_STREQ((char*)dequePopFront(ts), "-aux");
    ASSERT_STREQ((char*)dequePopFront(ts), "|");
    ASSERT_STREQ((char*)dequePopFront(ts), "grep");
    ASSERT_STREQ((char*)dequePopFront(ts), "abc ");
    ASSERT_STREQ((char*)dequePopFront(ts), ">");
    ASSERT_STREQ((char*)dequePopFront(ts), "a.txt");
}