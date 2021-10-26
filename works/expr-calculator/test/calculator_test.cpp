#include "calculator.hpp"

#include <gtest/gtest.h>
#include <sstream>

namespace calc::calculator {

namespace {
Calculator BuildCalculator(std::string some_str) {
    tokenizer::Tokenizer tokenizer(std::make_unique<std::istringstream>(some_str));
    return Calculator(std::move(tokenizer));
}
}  // namespace

TEST(Calculator, Ctor) {
    Calculator calc = BuildCalculator("1 + 2");
}

TEST(Calculator, JustNum) {
    Calculator calc = BuildCalculator("123");
    ASSERT_EQ(123, calc.Eval());

    std::string number = "123123123123123123123123";
    Calculator another_calc = BuildCalculator(number);
    ASSERT_EQ(number, another_calc.Eval().ToString());
}

TEST(Calculator, AdditiveOperations) {
    Calculator calc = BuildCalculator("123+456 - 8+0   -        1");
    ASSERT_EQ(570, calc.Eval());
}

TEST(Calculator, MultiplicalyOperations) {
    Calculator calc = BuildCalculator("2*2*2/3*22%7");
    ASSERT_EQ(2, calc.Eval());
}

TEST(Calculator, MixedTest) {
    Calculator calc = BuildCalculator("(3 + 4) * 5");
    EXPECT_EQ(35, calc.Eval());

    calc = std::move(BuildCalculator("4 + (4 * 5)"));
    EXPECT_EQ(24, calc.Eval());

    calc = std::move(BuildCalculator("22 + 16 / 4 - 4 * (17 - 2 * 7 + 3) + 7 * (3 + 4)"));
    EXPECT_EQ(51, calc.Eval());
}

TEST(Calculator, BigResult) {
    std::string num = "2";
    for (int i = 0; i < 100; ++i) {
        num.push_back('*');
        num.push_back('2');
    }
    Calculator calc = BuildCalculator(num);
    EXPECT_EQ("2535301200456458802993406410752", calc.Eval().ToString());
}

}  // namespace calc::calculator
