#include <calculator.hpp>

namespace calc::calculator {

namespace {

using Token = tokenizer::Token;

}

Calculator::Calculator(tokenizer::Tokenizer&& tokenizer) : tokenizer_(std::move(tokenizer)) {
}

big_numbers::BigInteger Calculator::Eval() {
  return CalcSum();
}

Calculator& Calculator::operator=(Calculator&& other) {
  tokenizer_ = std::move(other.tokenizer_);
  return *this;
}

big_numbers::BigInteger Calculator::CalcMult(){
    Number lhs = GetNumber();
    Token cur_token = tokenizer_.GetToken();
    tokenizer::MulOpToken* op_ptr = std::get_if<tokenizer::MulOpToken>(&cur_token);

    while (!tokenizer_.IsEnd() && op_ptr) {
        tokenizer_.Next();
        Number rhs = GetNumber();

        if (*op_ptr == tokenizer::MulOpToken::kMult) {
            lhs *= rhs;
        } else if (*op_ptr == tokenizer::MulOpToken::kDiv) {
            lhs = lhs / rhs;
        } else if (*op_ptr == tokenizer::MulOpToken::kModule) {
            lhs = lhs % rhs;
        }

        cur_token = tokenizer_.GetToken();
        op_ptr = std::get_if<tokenizer::MulOpToken>(&cur_token);
    }

    return lhs;
}

big_numbers::BigInteger Calculator::CalcSum(){
    Number lhs = CalcMult();
    Token cur_token = tokenizer_.GetToken();
    tokenizer::AddOpToken* op_ptr = std::get_if<tokenizer::AddOpToken>(&cur_token);

    while (!tokenizer_.IsEnd() && op_ptr) {

        tokenizer_.Next();
        Number rhs = CalcMult();

        if (*op_ptr == tokenizer::AddOpToken::kPlus) {
            lhs += rhs;
        } else if (*op_ptr == tokenizer::AddOpToken::kMinus) {
            lhs -= rhs;
        }

        cur_token = tokenizer_.GetToken();
        op_ptr = std::get_if<tokenizer::AddOpToken>(&cur_token);
    }

    return lhs;
}

big_numbers::BigInteger Calculator::CalcSubExpr(){
  Token cur_token = tokenizer_.GetToken();
  tokenizer::BracketToken* bracket_ptr = std::get_if<tokenizer::BracketToken>(&cur_token);

  if (tokenizer_.IsEnd()) {
      return 0;
  }
  if (!bracket_ptr || *bracket_ptr != tokenizer::BracketToken::kOpen) {
      throw std::runtime_error("Expected (");
  }

  tokenizer_.Next();
  Number res = CalcSum();

  cur_token = tokenizer_.GetToken();
  bracket_ptr = std::get_if<tokenizer::BracketToken>(&cur_token);
  if (tokenizer_.IsEnd() || !bracket_ptr || *bracket_ptr != tokenizer::BracketToken::kClose) {
      throw std::runtime_error("Expected )");
  }
  tokenizer_.Next();

  return res;
}

big_numbers::BigInteger Calculator::GetNumber() {
    if (tokenizer_.IsEnd()) {
        throw std::runtime_error("Expression ends unexpectedly. Number or ( is expected");
    }
    Token cur_token = tokenizer_.GetToken();
    tokenizer::NumberToken* number_ptr = std::get_if<tokenizer::NumberToken>(&cur_token);

    if (!number_ptr) {
        tokenizer::BracketToken* bracket_ptr = std::get_if<tokenizer::BracketToken>(&cur_token);

        return CalcSubExpr();

        if (!bracket_ptr) {
          throw std::runtime_error("Unexpected token. Number or ( is expected");
        }
    }

    tokenizer_.Next();
    return number_ptr->value;
}

}  // namespace calc::calculator
