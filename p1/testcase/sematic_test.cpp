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
    char **array = NULL;

    ASSERT_EQ(ss->count, 2);

    s = (stage*)ss->popFront(ss);
    array = (char**)s->argStack->cloneToArray(s->argStack);
    ASSERT_EQ(s->argStack->count , 2);
    ASSERT_EQ(s->in, STDIN_NORMAL);
    ASSERT_EQ(s->out, STDOUT_PIPED);
    ASSERT_EQ(array[2], (char*)NULL);
    ASSERT_STREQ(array[0], "ps");
    ASSERT_STREQ(array[1], "-aux");
    s->del(s);
    freeArray((dataptr*)array);

    s = (stage*)ss->popFront(ss);
    array = (char**)s->argStack->cloneToArray(s->argStack);
    ASSERT_EQ(s->argStack->count , 2);
    ASSERT_EQ(s->in, STDIN_PIPED);
    ASSERT_EQ(s->out, STDOUT_FILE_TRUNCATE);
    ASSERT_STREQ(s->stdoutArg, "a.txt");
    ASSERT_EQ(array[2], (char*)NULL);
    ASSERT_STREQ(array[0], "grep");
    ASSERT_STREQ(array[1], "abc ");

    s->del(s);
    freeArray((dataptr*)array);

    ASSERT_TRUE(ss->isEmpty(ss));

    ss->del(ss);
}

TEST(SEMANTIC, SIMPLE) {
    const char *cmd = "pwd";
    tokenStack *ts = NEW(tokenStack)();
    stageStack *ss = NEW(stageStack)();
    tokenize(cmd, ts);
    parse(ts, ss);

    stage *s = NULL;
    char **array = NULL;

    ASSERT_EQ(ss->count, 1);

    s = (stage*)ss->popFront(ss);
    array = (char**)s->argStack->cloneToArray(s->argStack);
    ASSERT_EQ(s->argStack->count , 1);
    ASSERT_EQ(s->in, STDIN_NORMAL);
    ASSERT_EQ(s->out, STDOUT_NORMAL);
    ASSERT_STREQ(array[1], NULL);
    ASSERT_STREQ(array[0], "pwd");
    s->del(s);
    freeArray((dataptr*)array);

    ASSERT_TRUE(ss->isEmpty(ss));

    ss->del(ss);
}

TEST(SEMANTIC, SIMPLE2) {
    const char *cmd = "cd /etc/next";
    tokenStack *ts = NEW(tokenStack)();
    stageStack *ss = NEW(stageStack)();
    tokenize(cmd, ts);
    parse(ts, ss);

    stage *s = NULL;
    char **array = NULL;

    ASSERT_EQ(ss->count, 1);

    s = (stage*)ss->popFront(ss);
    array = (char**)s->argStack->cloneToArray(s->argStack);
    ASSERT_EQ(s->argStack->count , 2);
    ASSERT_EQ(s->in, STDIN_NORMAL);
    ASSERT_EQ(s->out, STDOUT_NORMAL);
    ASSERT_STREQ(array[2], NULL);
    ASSERT_STREQ(array[0], "cd");
    ASSERT_STREQ(array[1], "/etc/next");
    s->del(s);
    freeArray((dataptr*)array);

    ASSERT_TRUE(ss->isEmpty(ss));

    ss->del(ss);
}

TEST(SEMANTIC, REDIRECT) {
    const char *cmd = ">> 1.txt cd < 2.txt /etc/next";
    tokenStack *ts = NEW(tokenStack)();
    stageStack *ss = NEW(stageStack)();
    tokenize(cmd, ts);
    parse(ts, ss);

    stage *s = NULL;
    char **array = NULL;

    ASSERT_EQ(ss->count, 1);

    s = (stage*)ss->popFront(ss);
    array = (char**)s->argStack->cloneToArray(s->argStack);
    ASSERT_EQ(s->argStack->count , 2);

    ASSERT_EQ(s->in, STDIN_FILE);
    ASSERT_STREQ(s->stdinArg, "2.txt");

    ASSERT_EQ(s->out, STDOUT_FILE_APPEND);
    ASSERT_STREQ(s->stdoutArg, "1.txt");

    ASSERT_EQ(array[2], (char*)NULL);
    ASSERT_STREQ(array[0], "cd");
    ASSERT_STREQ(array[1], "/etc/next");

    s->del(s);
    freeArray((dataptr*)array);

    ASSERT_TRUE(ss->isEmpty(ss));

    ss->del(ss);
}