#include "tokenizer.hpp"
#include <iostream>

namespace calc::tokenizer {

namespace {

int ToDigit(char c) {
    return c - '0';
}

}  // namespace

Tokenizer::Tokenizer(std::istream* input) : input_(input) {
    Next();
}

Tokenizer::Tokenizer(std::unique_ptr<std::istream>&& input)
    : u_ptr_(std::move(input)), input_(u_ptr_.get()) {
    Next();
}

Tokenizer::Tokenizer(Tokenizer&& other)
    : u_ptr_(std::move(other.u_ptr_)),
      input_(u_ptr_ ? u_ptr_.get() : other.input_),
      cur_token_(other.cur_token_),
      is_end_(other.is_end_) {
    other.input_ = nullptr;
    other.u_ptr_ = nullptr;
}

Tokenizer& Tokenizer::operator=(Tokenizer&& other) {
    u_ptr_ = std::move(other.u_ptr_);
    input_ = u_ptr_ ? u_ptr_.get() : other.input_;
    other.input_ = nullptr;
    other.u_ptr_ = nullptr;
    cur_token_ = other.cur_token_;
    is_end_ = other.is_end_;

    return *this;
}

void Tokenizer::SkipEmpty() {
    while (!StreamEnd() && input_->peek() == ' ') {
        input_->get();
    }
}

bool Tokenizer::StreamEnd() {
    return input_->eof() || input_->peek() == '\n';
}

bool Tokenizer::IsEnd() {
    return is_end_;
}

void Tokenizer::Next() {
    SkipEmpty();
    is_end_ = StreamEnd();

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
        while (!StreamEnd() && std::isdigit(input_->peek())) {
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
