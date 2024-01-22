#pragma once
#include "Combinator.hpp"
#include "Primitive.hpp"

namespace Parser {

template <typename T, typename E>
inline Parser<T, E> wrapped(Parser<T, E> const &parser, Token::Punctuation l,
                            Token::Punctuation r) {
	return token_punctuation(l) >> parser << token_punctuation(r);
}

template <typename T, typename E>
inline Parser<T, E> wrapped(Parser<T, E> const &parser, Token::Operator l, Token::Operator r) {
	return token_operator(l) >> parser << token_operator(r);
}

template <typename T, typename E> inline Parser<T, E> parenthesized(Parser<T, E> const &parser) {
	return wrapped(parser, Token::Punctuation::L_PAREN, Token::Punctuation::R_PAREN);
}

template <typename T, typename E> inline Parser<T, E> bracketed(Parser<T, E> const &parser) {
	return wrapped(parser, Token::Punctuation::L_BRACKET, Token::Punctuation::R_BRACKET);
}

template <typename T, typename E> inline Parser<T, E> braced(Parser<T, E> const &parser) {
	return wrapped(parser, Token::Punctuation::L_BRACE, Token::Punctuation::R_BRACE);
}

template <typename T, typename E> inline Parser<T, E> angled(Parser<T, E> const &parser) {
	return wrapped(parser, Token::Operator::LT, Token::Operator::GT);
}

template <typename T, typename E>
inline Parser<std::vector<T>, E> separated_by_comma(Parser<T, E> const &parser) {
	return separated<T, Token, E, ParserError, E>(parser,
	                                              token_punctuation(Token::Punctuation::COMMA));
}

};  // namespace Parser
