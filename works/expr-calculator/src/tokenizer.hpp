#pragma once

#include <istream>
#include <variant>
#include <memory>

#include <big_integer.hpp>

namespace calc::tokenizer {

struct NumberToken {
    big_numbers::BigInteger value;
};

enum class BracketToken { kOpen, kClose };

enum class AddOpToken { kPlus, kMinus };

enum class MulOpToken { kMult, kDiv, kModule };

using Token = std::variant<NumberToken, BracketToken, AddOpToken, MulOpToken>;

class Tokenizer {
public:
    Tokenizer() = delete;
    Tokenizer(std::unique_ptr<std::istream>);

    Tokenizer(Tokenizer&&);
    Tokenizer(const Tokenizer&) = delete;

    Tokenizer& operator=(Tokenizer&& other);
    Tokenizer& operator=(const Tokenizer&) = delete;

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    void SkipEmpty();

    std::unique_ptr<std::istream> input_;
    bool is_end_{false};
    Token cur_token_{};
};

}  // namespace calc::tokenizer
