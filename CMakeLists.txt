cmake_minimum_required(VERSION 3.6)
project(VE482)

set(FLAG_WERROR FALSE)
if (FLAG_WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -Wall -Wextra -Werror")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -Wall -Wextra")
endif()

    include_directories(lib/GTest/googletest/include)

add_subdirectory(lib/GTest)

add_subdirectory(p1)
add_subdirectory(h3)
add_subdirectory(l4)