#pragma once
#include "Parser.hpp"

namespace Parser {

template <typename T, typename E> inline Parser<T, E> constant(T const &value) {
	return [=](Stream<Token> &) { return value; };
}

Parser<Token, ParserError> satisfy(std::function<bool(Token const &)> check);

Parser<Token, ParserError> token_kind(Token::Kind kind);
inline Parser<Token, ParserError> token_identifier() { return token_kind(Token::Kind::IDENTIFIER); }
Parser<Token, ParserError> token_keyword(Token::Keyword keyword);
Parser<Token, ParserError> token_operator(Token::Operator operator_);
Parser<Token, ParserError> token_punctuation(Token::Punctuation punctuation);

};  // namespace Parser
