#pragma once
#include "Parser.hpp"

namespace Parser {

template <typename T, typename E> Parser<T, E> constant(T const &value);

Parser<Token, ParserError> satisfy(std::function<bool(Token)> check);

Parser<Token, ParserError> token_kind(Token::Kind kind);

};  // namespace Parser
