#pragma once
#include "Parser.hpp"

namespace Parser {

template <typename T> Parser<T> constant(T const &value);

Parser<Token> satisfy(std::function<bool(Token)> check);

Parser<Token> token_kind(Token::Kind kind);

};  // namespace Parser
