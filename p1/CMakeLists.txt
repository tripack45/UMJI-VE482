#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_FILES)

list(APPEND SOURCE_FILES src/main.c)

add_subdirectory(testcase)

# The default target
add_executable(p1 ${SOURCE_FILES})