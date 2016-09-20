//
// Created by tripack on 16-9-18.
//

#ifndef VE482_EXCEPTION_H
#define VE482_EXCEPTION_H

void setError(int e);
void resetError();
int errcode();

#define NO_ASSERT
#ifndef NO_ASSERT
    #define RAISE(e, ret) {assert(0); setError(e); return ret;}
    #define RAISE_VOID(e) {assert(0); setError(e); return;}
#else
    #define RAISE(e, ret) {setError(e); return ret;}
    #define RAISE_VOID(e) {setError(e); return;}
#endif

#define CATCH(e) if (errcode() == e)
#define CATCH_ELSE() if(errcode())


#define EXCEPTION_DEQUE_EMPTY 0xFF01
#define EXCEPTION_DEQUE_FULL 0xFF02
#define EXCEPTION_DEQUE_ILLEGAL_CTOR 0xFF03
#define EXCEPTION_DEQUE_NONE_EMPTY_DTOR 0xFF04E

#define EXCEPTION_TOKENIZER_TOO_MANY_CHAR 0xEE01
#define EXCEPTION_TOKENIZER_TOO_MANY_TOKEN 0xEE02
#define EXCEPTION_TOKENIZER_UNBALANCED_QUOTE 0xEE03

#define EXCEPTION_PASER_TOO_MANY_STDOUT_REDIR 0xEE04
#define EXCEPTION_PASER_TOO_MANY_STDIN_REDIR 0xEE05
#define EXCEPTION_PASER_REDIRECTION_PIPE_CONFLICT 0xEE06
#define EXCEPTION_PASER_MISSING_REDIRECTION_FILE 0xEE07

#define EXCEPTION_PASER_MISSING_EXECUTABLE 0xEE08

#endif //VE482_EXCEPTION_H
