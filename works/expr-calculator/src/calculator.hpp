#pragma once 
#include "tokenizer.hpp"

#include <big_integer.hpp>

namespace calc::calculator {

class Calculator {
private:
    using Number = big_numbers::BigInteger;

public:
    Calculator(tokenizer::Tokenizer&&);

    Calculator& operator=(Calculator&& other);

    Number Eval();

private:
    tokenizer::Tokenizer tokenizer_;

    Number CalcMult();
    Number CalcSum();
    Number CalcSubExpr();
    Number GetNumber();
};

}  // namespace calc::calculator
