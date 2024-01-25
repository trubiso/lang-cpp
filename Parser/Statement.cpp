#include "Statement.hpp"

namespace Parser {

Parser<Statement, ParserError> statement_create() {
	auto t = token_keyword(Token::Keyword::LET) | type();
	auto i = identifier();
	auto o = token_operator(Token::Operator::EQ);
	auto r = expression();
	auto stmt = ((t & i) << o) & r;
	// TODO: transform
	return transform(
	    stmt,
	    [](std::tuple<std::tuple<std::variant<Token, Type>, Identifier>, Expression> const &data) {
		    std::tuple<std::variant<Token, Type>, Identifier> const &l = std::get<0>(data);
		    std::variant<Token, Type> const &type_raw = std::get<0>(l);
		    Type type = std::holds_alternative<Token>(type_raw)
		                    ? Type{.kind = Type::Kind::INFERRED, .value = std::monostate{}}
		                    : std::get<Type>(type_raw);
		    Identifier const &identifier = std::get<1>(l);
		    Expression const &value = std::get<1>(data);
		    return Statement{.kind = Statement::Kind::CREATE,
		                     .value = Statement::Create{
		                         .type = type,
		                         .identifier = identifier,
		                         .mutable_ = false,  // TODO: mutable
		                         .value = value      // TODO: no value
		                     }};
	    });
}

Parser<Statement, ParserError> statement() {
	return statement_create();  // TODO: the rest of them
}

};  // namespace Parser
