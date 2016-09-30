//
// Created by tripack on 16-9-18.
//

#include <gtest/gtest.h>
#include "../src/oop.h"

extern "C" {
#include "../src/tokenizer.h"
}

TEST(TOKENIZER, SIMPLE) {
    const char *cmd = "ls";
    tokenStack *ts = NEW(tokenStack)();
    tokenize(cmd, ts);
    token *t0 = (token*)ts->popFront(ts);
    ASSERT_EQ(t0->type, TOKEN_STRING);
    ASSERT_STREQ(t0->content, "ls");
    t0->del(t0);
    ts->del(ts);
}

TEST(TOKENIZER, BLANK) {
    const char *cmd0 = "";
    const char *cmd1 = " ";
    const char *cmd2 = "    ";
    const char *cmd3 = "    ";
    tokenStack *ts0 = NEW(tokenStack)();
    tokenStack *ts1 = NEW(tokenStack)();
    tokenStack *ts2 = NEW(tokenStack)();
    tokenStack *ts3 = NEW(tokenStack)();
    tokenize(cmd0, ts0);
    tokenize(cmd1, ts1);
    tokenize(cmd2, ts2);
    tokenize(cmd3, ts3);
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
    const char *cmd = "  ls  -al    | grep abc | ls  -a  -b   >    123.txt >> \">\" < \" < \"  ";
    tokenStack *ts = NEW(tokenStack)();
    tokenize(cmd, ts);
    token *s = NULL;
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "ls");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "-al");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_PIPE); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "grep");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "abc");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_PIPE); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "ls");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "-a");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "-b");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_REDIR_STDOUT_TRUNC); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "123.txt");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_REDIR_STDOUT_APPEND); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, ">");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_REDIR_STDIN); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, " < ");
    ASSERT_TRUE(ts->isEmpty(ts));
    ts->del(ts);;
}

TEST(TOKENIZER, COMPLEX) {
    const char *cmd = "ps -aux | grep \"abc \" > a.txt";
    tokenStack *ts = NEW(tokenStack)();
    tokenize(cmd, ts);
    token *s = NULL;

    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "ps");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "-aux");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_PIPE); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "grep");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "abc ");
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_REDIR_STDOUT_TRUNC); ASSERT_STREQ(s->content, NULL);
    s = (token*)ts->popFront(ts);
    ASSERT_EQ(s->type, TOKEN_STRING); ASSERT_STREQ(s->content, "a.txt");
}