project(big-integer-source)

add_library(big-integer_lib STATIC big_integer.hpp big_integer.cpp)

add_executable(big-integer_exe main.cpp)

target_link_libraries(big-integer_exe PUBLIC big-integer_lib)
