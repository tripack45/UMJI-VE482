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
    char *t0 = (char*)ts->popFront(ts);
    ASSERT_STREQ(t0, "ls");
    ts->del(ts);
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
    ASSERT_TRUE(ts0->isEmpty(ts0));
    ASSERT_TRUE(ts1->isEmpty(ts1));
    ASSERT_TRUE(ts2->isEmpty(ts2));
    ASSERT_TRUE(ts3->isEmpty(ts3));
    ts0->del(ts0);;
    ts1->del(ts1);;
    ts2->del(ts2);;
    ts3->del(ts3);;
}

TEST(TOKENIZER, MULTI) {
    const char *cmd = "  ls  -al    | grep abc | ls  -a  -b   >    123.txt  ";
    tokenStack *ts = tokenize(cmd);
    ASSERT_STREQ((char*)ts->popFront(ts), "ls");
    ASSERT_STREQ((char*)ts->popFront(ts), "-al");
    ASSERT_STREQ((char*)ts->popFront(ts), "|");
    ASSERT_STREQ((char*)ts->popFront(ts), "grep");
    ASSERT_STREQ((char*)ts->popFront(ts), "abc");
    ASSERT_STREQ((char*)ts->popFront(ts), "|");
    ASSERT_STREQ((char*)ts->popFront(ts), "ls");
    ASSERT_STREQ((char*)ts->popFront(ts), "-a");
    ASSERT_STREQ((char*)ts->popFront(ts), "-b");
    ASSERT_STREQ((char*)ts->popFront(ts), ">");
    ASSERT_STREQ((char*)ts->popFront(ts), "123.txt");
    ASSERT_TRUE(ts->isEmpty(ts));
    
    ts->del(ts);;
}

TEST(TOKENIZER, QUOTE) {
    const char *cmd = "  ls  -al \"| < a.txt\"   | grep abc | ls  -a  -b   >    123.txt  ";
    tokenStack *ts = tokenize(cmd);
    ASSERT_STREQ((char*)ts->popFront(ts), "ls");
    ASSERT_STREQ((char*)ts->popFront(ts), "-al");
    ASSERT_STREQ((char*)ts->popFront(ts), "| < a.txt");
    ASSERT_STREQ((char*)ts->popFront(ts), "|");
    ASSERT_STREQ((char*)ts->popFront(ts), "grep");
    ASSERT_STREQ((char*)ts->popFront(ts), "abc");
    ASSERT_STREQ((char*)ts->popFront(ts), "|");
    ASSERT_STREQ((char*)ts->popFront(ts), "ls");
    ASSERT_STREQ((char*)ts->popFront(ts), "-a");
    ASSERT_STREQ((char*)ts->popFront(ts), "-b");
    ASSERT_STREQ((char*)ts->popFront(ts), ">");
    ASSERT_STREQ((char*)ts->popFront(ts), "123.txt");
    ASSERT_TRUE(ts->isEmpty(ts));
    
    ts->del(ts);;
}


TEST(TOKENIZER, COMPLEX) {
    const char *cmd = "ps -aux | grep \"abc \" > a.txt";
    tokenStack *ts = tokenize(cmd);

    ASSERT_STREQ((char*)ts->popFront(ts), "ps");
    ASSERT_STREQ((char*)ts->popFront(ts), "-aux");
    ASSERT_STREQ((char*)ts->popFront(ts), "|");
    ASSERT_STREQ((char*)ts->popFront(ts), "grep");
    ASSERT_STREQ((char*)ts->popFront(ts), "abc ");
    ASSERT_STREQ((char*)ts->popFront(ts), ">");
    ASSERT_STREQ((char*)ts->popFront(ts), "a.txt");
}