cmake_minimum_required(VERSION 3.6)
project(VE482)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(lib/GTest/googletest/include)

add_subdirectory(lib/GTest)

add_subdirectory(p1)

add_subdirectory(h3)