#include "Tokenizer.hpp"

#include <cctype>

std::optional<Token> Tokenizer::next() {
	consume_whitespace();
	if (!current().has_value()) return {};
	if (std::isalpha(current().value())) return consume_identifier();
}

void Tokenizer::consume_whitespace() noexcept {
	while (is_index_valid() && std::isblank(current().value())) {
		advance();
	}
}

Token Tokenizer::consume_identifier() noexcept {
	size_t start = m_index;
	while (is_index_valid() && std::isalnum(current().value())) {
		advance();
	}
	size_t end = m_index - 1;
	return Token{.kind = Token::Kind::IDENTIFIER, .span = Span{.start = start, .end = end}};
}
