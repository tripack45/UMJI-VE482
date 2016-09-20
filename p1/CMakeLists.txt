#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_FILES
        src/main.c
        src/deque.c
        src/exception.c
        src/tokenizer.c
        src/parser.c
        src/signal_action.c)

# The default target
add_executable(p1 ${SOURCE_FILES})
add_library(libp1_test ${SOURCE_FILES})

add_subdirectory(testcase)