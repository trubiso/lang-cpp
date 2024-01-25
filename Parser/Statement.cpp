#include "Statement.hpp"

namespace Parser {

Parser<Statement, ParserError> statement_create() {
	auto t = token_keyword(Token::Keyword::LET) | type();
	auto i = identifier();
	auto o = token_operator(Token::Operator::EQ);
	auto r = expression();
	// TODO: transform
	return ((t & i) << o) & r;
}

Parser<Statement, ParserError> statement();

};
