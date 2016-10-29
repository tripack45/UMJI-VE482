#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Builing of dependent libraries

set(SOURCE_DS
        src/deque.c
        src/dictionary.c
        src/exception.c)

set(SOURCE_LIB_SORTTEXT
        src/libtext_sort.c)

set(SOURCE_CMD_DRIVER
        src/cmdmain.c
        src/utils.c)

set(SOURCE_UI_DRIVER
        src/uimain.c
        src/utils.c)

# The default target

add_library(l4_ds ${SOURCE_DS})
add_library(l4_sort ${SOURCE_LIB_SORTTEXT})

add_library(l4_ds_shared SHARED ${SOURCE_DS})
add_library(l4_sort_shared SHARED ${SOURCE_LIB_SORTTEXT})

add_executable(l4 ${SOURCE_CMD_DRIVER})
add_executable(l4_ui ${SOURCE_UI_DRIVER})

target_link_libraries(l4    l4_sort l4_ds)
target_link_libraries(l4_ui l4_sort l4_ds)
