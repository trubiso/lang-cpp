#pragma once
#include "Combinator.hpp"
#include "Primitive.hpp"

namespace Parser {

inline Parser<Token, ParserError> wrapped(Parser<Token, ParserError> const &parser,
                                          Token::Punctuation l, Token::Punctuation r) {
	return token_punctuation(l) >> parser << token_punctuation(r);
}

inline Parser<Token, ParserError> wrapped(Parser<Token, ParserError> const &parser,
                                          Token::Operator l, Token::Operator r) {
	return token_operator(l) >> parser << token_operator(r);
}

inline Parser<Token, ParserError> parenthesized(Parser<Token, ParserError> const &parser) {
	return wrapped(parser, Token::Punctuation::L_PAREN, Token::Punctuation::R_PAREN);
}

inline Parser<Token, ParserError> bracketed(Parser<Token, ParserError> const &parser) {
	return wrapped(parser, Token::Punctuation::L_BRACKET, Token::Punctuation::R_BRACKET);
}

inline Parser<Token, ParserError> braced(Parser<Token, ParserError> const &parser) {
	return wrapped(parser, Token::Punctuation::L_BRACE, Token::Punctuation::R_BRACE);
}

inline Parser<Token, ParserError> angled(Parser<Token, ParserError> const &parser) {
	return wrapped(parser, Token::Operator::LT, Token::Operator::GT);
}

};  // namespace Parser
