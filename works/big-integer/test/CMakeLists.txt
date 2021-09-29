project(big-integer-test)

add_executable(big-integer_test main.cpp big_integer_test.cpp)

add_test(NAME test-big-integer COMMAND big-integer_test) 

target_link_libraries(big-integer_test PUBLIC gtest big-integer_lib)
