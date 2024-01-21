#pragma once
#include "Combinator.hpp"
#include "Primitive.hpp"

namespace Parser {

Parser<Token, ParserError> parenthesized(Parser<Token, ParserError> const &parser) {
	return token_punctuation(Token::Punctuation::L_PAREN) >> parser <<
	       token_punctuation(Token::Punctuation::R_PAREN);
}

};  // namespace Parser
