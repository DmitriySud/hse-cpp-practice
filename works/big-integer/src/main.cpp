#include "big_integer.hpp"

#include <iostream>
#include <string>

int main() {
    BigInteger one, two;
    std::string op;

    std::cin >> one >> op >> two;

    BigInteger result;
    if (op == "-") {
        result = one - two;
    } else if (op == "+") {
        result = one + two;
    } else if (op == "*") {
        result = one * two;
    } else if (op == "/") {
        result = one / two;
    } else if (op == "%") {
        result = one % two;
    } else if (op == "<") {
        result = one < two;
    } else if (op == ">") {
        result = one > two;
    } else if (op == "<=") {
        result = one <= two;
    } else if (op == ">=") {
        result = one >= two;
    } else if (op == "==") {
        result = one == two;
    }

    std::cout << result << std::endl;

    return 0;
}
