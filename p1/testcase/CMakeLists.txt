add_executable(p1_test
        deque_test.cpp
        tokenizer_test.cpp
        sematic_test.cpp    )

target_link_libraries(p1_test
        gtest
        gtest_main
        libp1_test)