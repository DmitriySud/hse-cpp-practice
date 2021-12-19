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
    /// Use this constructor when you track objects lifetime yourself
    Tokenizer(std::istream*);
    /// Use this constructor when you can give objects ownership to this class
    Tokenizer(std::unique_ptr<std::istream>&&);

    Tokenizer(Tokenizer&&);
    Tokenizer(const Tokenizer&) = delete;

    Tokenizer& operator=(Tokenizer&& other);
    Tokenizer& operator=(const Tokenizer&) = delete;

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    void SkipEmpty();
    bool StreamEnd();

    std::unique_ptr<std::istream> u_ptr_{nullptr};
    std::istream* input_;
    Token cur_token_{};
    bool is_end_{false};
};

}  // namespace calc::tokenizer
