project(expr-calulator-test)

add_executable(expr-calculator_test tokenizer_test.cpp calculator_test.cpp)

add_test(NAME test-expr-calculator COMMAND expr-calculator_test)

target_link_libraries(expr-calculator_test PUBLIC gtest_main expr-calculator_lib)

