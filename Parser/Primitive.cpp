#include "Primitive.hpp"

namespace Parser {

Parser<Token, ParserError> satisfy(std::function<bool(Token const &)> check) {
	return [=](Stream<Token> &input) -> Result<Token, ParserError> {
		std::optional<Token> value = input.peek();
		if (!value.has_value()) return ParserError{};
		if (check(value.value())) {
			input.ignore();
			return value.value();
		} else {
			// TODO: fix
			return ParserError{};
		}
	};
}

Parser<Token, ParserError> token_kind(Token::Kind kind) {
	return satisfy([=](Token const &token) { return token.kind == kind; });
}

Parser<Token, ParserError> token_keyword(Token::Keyword keyword) {
	std::string desired;
	switch (keyword) {
	case Token::Keyword::LET:
		desired = "let";
		break;
	default:
		throw "???";
	}
	return satisfy([=](Token const &token) {
		if (token.kind != Token::Kind::IDENTIFIER) return false;
		std::string const &value = std::get<std::string>(token.value);
		return value == desired;
	});
}

Parser<Token, ParserError> token_operator(Token::Operator operator_) {
	return satisfy([=](Token const &token) {
		if (token.kind != Token::Kind::OPERATOR) return false;
		return std::get<Token::Operator>(token.value) == operator_;
	});
}

Parser<Token, ParserError> token_punctuation(Token::Punctuation punctuation) {
	return satisfy([=](Token const &token) {
		if (token.kind != Token::Kind::PUNCTUATION) return false;
		return std::get<Token::Punctuation>(token.value) == punctuation;
	});
}

};  // namespace Parser
