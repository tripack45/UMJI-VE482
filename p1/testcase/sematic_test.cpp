//
// Created by tripack on 16-9-18.
//

#include <gtest/gtest.h>

extern "C" {
#include "../src/tokenizer.h"
#include "../src/parser.h"
#include "../src/exception.h"
#include "../src/oop.h"
}
/*
TEST(SEMANTIC, ERROR_PIPE) {
    const char *cmd = "ls |"
    "ls < 123.txt | b < 123.txt"
    "< 123.txt"
    "> 123.txt abc | b"
    "ab arg1 arg2 < 123.txt > 123.txt | /bin/c"
    "ab arg1 arg2 >> 123.txt < 123.txt"
    "ab arg1 < 123.txt < 234.txt"
    "< 123.txt ab arg1 arg2 > 234.txt arg3"
    "ab arg1 > 123.txt abc < 123.txt"; // Input / Output is the same file

    tokenStack *ts = tokenize(cmd);
    char *t0 = (char*)ts->popFront(ts);
    ASSERT_STREQ(t0, "ls");
    dequeFreeAll(ts);
    deleteDeque(ts);
}*/

TEST(SEMANTIC, NORMAL0) {
    const char *cmd = "ps -aux | grep \"abc \" > a.txt";
    tokenStack *ts = NEW(tokenStack)();
    stageStack *ss = NEW(stageStack)();
    tokenize(cmd, ts);
    parse(ts, ss);

    stage *s = NULL;
    ASSERT_EQ(ss->count, 2);
    s = (stage*)ss->popFront(ss);
    ASSERT_EQ(s->argc , 2);
    ASSERT_EQ(s->stdin, STDIN_NORMAL);
    ASSERT_EQ(s->stdout, STDOUT_PIPED);
    ASSERT_EQ(s->argv[2], (char*)NULL);
    ASSERT_STREQ(s->argv[0], "ps");
    ASSERT_STREQ(s->argv[1], "-aux");

    s = (stage*)ss->popFront(ss);
    ASSERT_EQ(s->argc , 2);
    ASSERT_EQ(s->stdin, STDIN_PIPED);
    ASSERT_EQ(s->stdout, STDOUT_FILE_TRUNCATE);
    ASSERT_STREQ(s->stdoutArg, "a.txt");
    ASSERT_EQ(s->argv[2], (char*)NULL);
    ASSERT_STREQ(s->argv[0], "grep");
    ASSERT_STREQ(s->argv[1], "abc ");

    ASSERT_TRUE(ss->isEmpty(ss));
}

TEST(SEMANTIC, SIMPLE) {
    const char *cmd = "pwd";
    tokenStack *ts = tokenize(cmd);
    stageStack *ss = parse(ts);

    stage *s = NULL;
    ASSERT_EQ(ss->count, 1);

    s = (stage*)ss->popFront(ss);
    ASSERT_EQ(s->argc , 1);
    ASSERT_EQ(s->stdin, STDIN_NORMAL);
    ASSERT_EQ(s->stdout, STDOUT_NORMAL);
    ASSERT_EQ(s->argv[1], (char*)NULL);
    ASSERT_STREQ(s->argv[0], "pwd");

    ASSERT_TRUE(ss->isEmpty(ss));
}

TEST(SEMANTIC, SIMPLE2) {
    const char *cmd = "cd /etc/next";
    tokenStack *ts = tokenize(cmd);
    stageStack *ss = parse(ts);

    stage *s = NULL;
    ASSERT_EQ(ss->count, 1);

    s = (stage*)ss->popFront(ss);
    ASSERT_EQ(s->argc , 2);
    ASSERT_EQ(s->stdin, STDIN_NORMAL);
    ASSERT_EQ(s->stdout, STDOUT_NORMAL);
    ASSERT_EQ(s->argv[2], (char*)NULL);
    ASSERT_STREQ(s->argv[0], "cd");
    ASSERT_STREQ(s->argv[1], "/etc/next");

    ASSERT_TRUE(ss->isEmpty(ss));
}

TEST(SEMANTIC, REDIRECT) {
    const char *cmd = ">> 1.txt cd < 2.txt /etc/next";
    tokenStack *ts = tokenize(cmd);
    stageStack *ss = parse(ts);

    stage *s = NULL;
    ASSERT_EQ(ss->count, 1);

    s = (stage*)ss->popFront(ss);
    ASSERT_EQ(s->argc , 2);

    ASSERT_EQ(s->stdin, STDIN_FILE);
    ASSERT_STREQ(s->stdinArg, "2.txt");

    ASSERT_EQ(s->stdout, STDOUT_FILE_APPEND);
    ASSERT_STREQ(s->stdoutArg, "1.txt");

    ASSERT_EQ(s->argv[2], (char*)NULL);
    ASSERT_STREQ(s->argv[0], "cd");
    ASSERT_STREQ(s->argv[1], "/etc/next");

    ASSERT_TRUE(ss->isEmpty(ss));
}