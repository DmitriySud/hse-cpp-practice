#include "tokenizer.hpp"

#include <gtest/gtest.h>
#include <sstream>

namespace calc::tokenizer {

bool operator==(const NumberToken& lhs, const NumberToken& rhs) {
    return lhs.value == rhs.value;
}

TEST(Tokenizer, Ctor) {
    Tokenizer t{std::make_unique<std::istringstream>()};

    ASSERT_TRUE(t.IsEnd());
}

TEST(Tokenizer, CtorString) {
    Tokenizer t{std::make_unique<std::istringstream>("()")};

    ASSERT_FALSE(t.IsEnd());
}

TEST(Tokenizer, Expr) {
    Tokenizer t{std::make_unique<std::istringstream>("1 + 2 * 3 % (4 / 5)")};

    std::vector<Token> ans = {NumberToken{1},      AddOpToken::kPlus,   NumberToken{2},
                              MulOpToken::kMult,   NumberToken{3},      MulOpToken::kModule,
                              BracketToken::kOpen, NumberToken{4},      MulOpToken::kDiv,
                              NumberToken{5},      BracketToken::kClose};

    for (auto& item : ans) {
        ASSERT_FALSE(t.IsEnd());
        ASSERT_EQ(t.GetToken(), item);
        t.Next();
    }
}

TEST(Tokenizer, ExtraSpaces) {
    Tokenizer t{std::make_unique<std::istringstream>("  1321+2    *  3%    ( 4 / 5)      ")};

    std::vector<Token> ans = {NumberToken{1321},   AddOpToken::kPlus,   NumberToken{2},
                              MulOpToken::kMult,   NumberToken{3},      MulOpToken::kModule,
                              BracketToken::kOpen, NumberToken{4},      MulOpToken::kDiv,
                              NumberToken{5},      BracketToken::kClose};

    for (auto& item : ans) {
        ASSERT_FALSE(t.IsEnd());
        ASSERT_EQ(t.GetToken(), item);
        t.Next();
    }
}

}  // namespace calc::tokenizer
