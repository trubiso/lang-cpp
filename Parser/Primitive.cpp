#include "Primitive.hpp"

namespace Parser {

template <typename T> Parser<T> constant(T const &value) {
	return [=](Stream<Token> &) { return value; };
}

Parser<Token> satisfy(std::function<bool(Token)> check) {
	return [=](Stream<Token> &input) -> Result<Token, ParserError> {
		Token const &value = input.peek();
		if (check(value)) {
			input.ignore();
			return value;
		} else {
			// TODO: fix
			return ParserError{};
		}
	};
}

Parser<Token> token_kind(Token::Kind kind) {
	return [=](Stream<Token> &input) -> Result<Token, ParserError> {
		Token const &value = input.peek();
		if (value.kind == kind) {
			input.ignore();
			return value;
		} else {
			// TODO: fix
			return ParserError{};
		}
	};
}

};  // namespace Parser
