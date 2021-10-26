project(exp-calulator-source)

add_library(calculator_lib STATIC calculator.hpp calculator.cpp)
add_library(tokenizer_lib STATIC tokenizer.hpp tokenizer.cpp)

add_library(expr-calculator_lib STATIC fake.cpp)
target_link_libraries(expr-calculator_lib PUBLIC tokenizer_lib calculator_lib)
target_link_libraries(expr-calculator_lib PUBLIC big-integer_lib)

add_executable(expr-calculator_exe main.cpp)
target_link_libraries(expr-calculator_exe PUBLIC expr-calculator_lib)

