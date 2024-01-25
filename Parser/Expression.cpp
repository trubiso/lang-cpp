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
	       expression_char_literal() | lazy(parenthesized(expression()));
}

// TODO: we require foldl and foldr
#define BINARY_OPERATOR(OP, NEXT)                                                               \
	transform(                                                                                  \
	    NEXT &many(OP &NEXT),                                                                   \
	    [](std::tuple<Expression, std::vector<std::tuple<Token::Operator, Expression>>> const   \
	           &data) {                                                                         \
		    Expression l = std::get<0>(data);                                                   \
		    for (std::tuple<Token::Operator, Expression> const &pair : std::get<1>(data)) {     \
			    Token::Operator const &operator_ = std::get<0>(pair);                           \
			    Expression const &r = std::get<1>(pair);                                        \
			    Expression::BinaryOperation operation =                                         \
			        Expression::BinaryOperation{.l = l, .r = r, .operator_ = operator_};        \
			    l = Expression{.kind = Expression::Kind::BINARY_OPERATION, .value = operation}; \
		    }                                                                                   \
		    return l;                                                                           \
	    })

#define OPERATOR(OP) token_operator(OP) >> constant(OP)

Parser<Expression, ParserError> expression() {
	using enum Token::Operator;
	auto neg = OPERATOR(NEG);
	auto plus_neg = OPERATOR(PLUS) | OPERATOR(NEG);
	auto star_div = OPERATOR(STAR) | OPERATOR(DIV);

	auto binary_star_div = BINARY_OPERATOR(star_div, expression_atom());
	auto binary_plus_neg = BINARY_OPERATOR(plus_neg, binary_star_div);

	return binary_plus_neg;
}

};  // namespace Parser
