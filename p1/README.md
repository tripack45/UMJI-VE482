# README

VE482 shell v1.0
Author: Yao Yue
SID: 5143709219
Release date: 3/10/2016

Please note this is a Markdown text file. Please open with markdown editors for better readability. 

## Introduction

The VE482 shell is a shell implemented with C. It is working both under linux and minix. 

To compile and use this sofware, run:
```
cc -o src/api.c src/deque.c src/exception.c src/execute.c src/main.c src/parser.c src/tokenizer.c && ./p1
```

## Features

* Object Oriented Programming style

The shell is implement in C, but in an object oriented style. This project use function pointers to mimic the idea of "method", "inheritence" and achieve "polymorphism". Using these techniques it will be easier to manage memory and trace memory leaks problems. It also more sematically clear.

* Full Error Handling

This implementation deals with errors and exceptions in every stage. Using macros and global variables, I am able to implement a psuedo-SEH. 

* Full feature support. 
  Test as you like. This implementation support all tasks specified  in the assianment requirement and works properly both on linux and minix.

* Standard "Read-Parse-Execute" loop

This shell is implemented using a standard "read parse execute" loop. The functions 

1. `main.c`: The main program. Glues everything together
2. `api.c` handles everything that is platform specific. Wraps them in functions. This provides an abstraction layer. `api.c` manages currently running processing using a `context`. 
3. `tokenizer.c` implements a simple input reader and a tokenizer. The tokenizer breaks the input string into smaller *tokens*, and push them into a *tokenstack*. A token has a *type* and a *content*. The type field is used to differentiate normal strings and various operators (">", |", etc.). This stage deals with the quotes.
4. `parser.c` works on the tokenstack resulted from the tokenizer. It performs semantic analysis on the token stream. The token string will be parsed into a stream of *stages*, called *stagestack*. A stage represents one proecess to execute. A stage contains the *argv* of the program (including name and arguments), it's input and output file (rediretions). Note that a stage can have at most one input/output redirection. Meaning comand like `echo 123 > a.txt | grep 123` is considered illeagal, because the output of the first command is redirected to 2 places.
5. `execute.c` executes the stagestack resulted from parsing. Each call to `execute.c` will create its own *context*. There are two different type of commands, builtin type and external type. Although both of them will run in sperate process, builtin commands will not need to call `exec()` functions.

* Good Test Coverage

  All parts in 3) 4) 5) are subject to unit tests. you can find them under `testcase/`. This project uses *Google Tests* framework. 

### Special Notes:

We need to note the behavior of following commands:

* `echo 123 > 1.txt | grep 1`
  This command will cause a syntax error. This implementation does not allow the output of one file to be redirected to more then 1 place. This behavior different from bash.
* `echo 123 | < 1.txt cat`
  This command will cause a syntax error. For similar reason we believe redirecting stdin from multiple source simply does not make sense. This behavior is different from bash
* `echo 123 > 1.txt abc` 
  This command will write "123 abc" into file "1.txt". The redirection command is allowed to appear at any place in the inputed command. This behavior is consistent with bash. 
* `echo 123 > >`  and `echo 123 > ">"`
  The first one will cause an syntax error, while the latter one will write "123" into a file name ">". This shell forbids using unquoted operator as filename. But the latter one is valid. This behavior is consistent with bash.
* Pressing `Ctrl-D`
  When nothing is inputed, the shell exits immediately. When something is inputed, first key stroke does nothing. Second one clears all input.  The latter behavior is different from bash.
* Incomplete command
  When the user enter something that ends with "|", ">", ">>", "<" (one of the operators), the shell waits the user to complete his command. The bash only waits for "|", however the assignment mentions ">" and "|". I think it is only logical to wait  all of them, not some of them. This behavior is different from bash.
* `pwd > 1.txt`
  The output of `pwd` can be subjected to output redirection. This behavior is consistent with bash.
* The prompt detects whether you are on minix or linux :)