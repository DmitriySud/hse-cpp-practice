project(big-integer-test)

add_executable(big-integer_test big_integer_test.cpp)

add_test(NAME test-big-integer COMMAND big-integer_test) 

target_link_libraries(big-integer_test PUBLIC gtest_main big-integer_lib)
