#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_FILES src/utils.c src/utils.h)

list(APPEND SOURCE_FILES
        src/deque.c
        src/main.c
        src/dictionary.c
        src/exception.c)

add_subdirectory(testcase)

# The default target
add_executable(h3_ex3 ${SOURCE_FILES})