#include "tokenizer.hpp"

namespace calc::tokenizer {

namespace {

int ToDigit(char c) {
    return c - '0';
}

}  // namespace

Tokenizer::Tokenizer(std::unique_ptr<std::istream> input) : input_(std::move(input)) {
    Next();
}

Tokenizer::Tokenizer(Tokenizer&& other)
    : input_(std::move(other.input_)), is_end_(other.is_end_), cur_token_(other.cur_token_) {
}

// Tokenizer::Tokenizer(const Tokenizer& other)
//: input_(other.input_), is_end_(other.is_end_), cur_token_(other.cur_token_) {
// Next();
//}

Tokenizer& Tokenizer::operator=(Tokenizer&& other) {
    input_ = std::move(other.input_);
    cur_token_ = other.cur_token_;
    is_end_ = other.is_end_;

    return *this;
}

void Tokenizer::SkipEmpty() {
    while (!input_->eof() && input_->peek() == ' ') {
        input_->get();
    }
}

bool Tokenizer::IsEnd() {
    return is_end_;
}

void Tokenizer::Next() {
    SkipEmpty();
    is_end_ = input_->eof();

    if (is_end_) {
        return;
    }

    char cur_c = input_->get();
    if (cur_c == '(') {
        cur_token_ = BracketToken::kOpen;
    } else if (cur_c == ')') {
        cur_token_ = BracketToken::kClose;
    } else if (cur_c == '*') {
        cur_token_ = MulOpToken::kMult;
    } else if (cur_c == '/') {
        cur_token_ = MulOpToken::kDiv;
    } else if (cur_c == '%') {
        cur_token_ = MulOpToken::kModule;
    } else if (cur_c == '-') {
        cur_token_ = AddOpToken::kMinus;
    } else if (cur_c == '+') {
        cur_token_ = AddOpToken::kPlus;
    } else if (std::isdigit(cur_c)) {
        std::string res_str;
        res_str.push_back(cur_c);
        while (!input_->eof() && std::isdigit(input_->peek())) {
            cur_c = input_->get();
            res_str.push_back(cur_c);
        }
        cur_token_ = NumberToken{big_numbers::BigInteger(res_str)};
    }
}

Token Tokenizer::GetToken() {
    return cur_token_;
}

}  // namespace calc::tokenizer
