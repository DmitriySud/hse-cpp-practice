#include <iostream>
#include <string>
#include "calculator.hpp"

int main() {

    calc::tokenizer::Tokenizer tokenizer(&std::cin);
    calc::calculator::Calculator calc(std::move(tokenizer));

    std::cout << std::endl << "answer is " << calc.Eval() << std::endl;
}
