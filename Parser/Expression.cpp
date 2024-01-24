#include "Expression.hpp"

namespace Parser {

#define PARSER_LITERAL(T)                                               \
	transform(token_kind(Token::Kind::T), [](Token const &token) {      \
		return Expression{.kind = Expression::Kind::T,                  \
		                  .value = std::get<std::string>(token.value)}; \
	})

Parser<Expression, ParserError> expression_char_literal() { return PARSER_LITERAL(CHAR_LITERAL); }

Parser<Expression, ParserError> expression_string_literal() {
	return PARSER_LITERAL(STRING_LITERAL);
}

// TODO: CONSIDER: should numbers still be strings at this point?
Parser<Expression, ParserError> expression_number_literal() {
	return PARSER_LITERAL(NUMBER_LITERAL);
}

Parser<Expression, ParserError> expression_identifier() {
	return transform(qualified_identifier(), [](Identifier const &identifier) {
		return Expression{.kind = Expression::Kind::IDENTIFIER, .value = identifier};
	});
}

Parser<Expression, ParserError> expression_atom() {
	return expression_identifier() | expression_number_literal() | expression_string_literal() |
	       expression_char_literal();
}

// TODO: we require foldl and foldr
Parser<Expression, ParserError> expression_binary_operation();
Parser<Expression, ParserError> expression_unary_operation();

Parser<Expression, ParserError> expression() {
	return expression_binary_operation() | expression_unary_operation() | expression_atom();
}

};  // namespace Parser
