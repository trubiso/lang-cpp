#include <string>
#include <variant>

#include "../Box.hpp"
#include "Identifier.hpp"

namespace Parser {

struct Expression {
	// missing: CALL, DOT, CURRY(?), DEREF, REF, REF_RAW, LAMBDA, TYPE(?), SCOPE(?)
	enum class Kind {
		CHAR_LITERAL,
		STRING_LITERAL,
		NUMBER_LITERAL,
		IDENTIFIER,
		BINARY_OPERATION,
		UNARY_OPERATION,
	};

	struct BinaryOperation {
		Box<Expression> l;
		Box<Expression> r;
		Token::Operator operator_;
	};

	struct UnaryOperation {
		Box<Expression> value;
		Token::Operator operator_;
	};

	Kind kind;
	std::variant<std::string, Identifier, BinaryOperation, UnaryOperation> value;
};

Parser<Expression, ParserError> expression_char_literal();
Parser<Expression, ParserError> expression_string_literal();
Parser<Expression, ParserError> expression_number_literal();
Parser<Expression, ParserError> expression_identifier();
Parser<Expression, ParserError> expression_atom();

Parser<Expression, ParserError> expression();

};  // namespace Parser
