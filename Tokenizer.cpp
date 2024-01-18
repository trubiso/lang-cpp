#include "Tokenizer.hpp"

#include <cctype>

std::optional<Token> Tokenizer::next() {
	consume_whitespace();
	if (!current().has_value()) return {};
	if (std::isalpha(current().value())) return consume_identifier();
	switch (current().value()) {
	case '"':
		return consume_string_literal();
	case '\'':
		return consume_char_literal();
	case '+':
	case '-':
	case '*':
	case '/':
	case '&':
	case '|':
	case '^':
	case '~':
	case '=':
	case '<':
	case '>':
	case '!':
	case '?': // FIXME: disallow ?= (what even is that)
		return consume_operator();
	// TODO: arrows (must be handled in operators)
	case '(':
	case ')':
	case '[':
	case ']':
	case '{':
	case '}':
	case '.':
	case ',':
	case ':':
	case ';':
		return consume_punctuation();
	}
	return {};
}

void Tokenizer::consume_whitespace() noexcept {
	while (is_index_valid() && is_whitespace(current().value())) {
		advance();
	}
}

std::optional<Token> Tokenizer::consume_identifier() noexcept {
	size_t start = m_index;
	while (is_index_valid() && std::isalnum(current().value())) {
		advance();
	}
	size_t end = m_index;
	return Token{.kind = Token::Kind::IDENTIFIER, .span = Span{.start = start, .end = end}};
}

std::optional<Token> Tokenizer::consume_number_literal() noexcept {
	// TODO
	return {};
}

std::optional<Token> Tokenizer::consume_string_literal() noexcept {
	size_t start = m_index;
	advance();  // consume "
	while (is_index_valid() && current().value() != '"') advance();
	advance();                           // consume "
	std::ignore = consume_identifier();  // just in case it ends with a postfix
	size_t end = m_index;
	return Token{.kind = Token::Kind::STRING_LITERAL, .span = Span{.start = start, .end = end}};
}

std::optional<Token> Tokenizer::consume_char_literal() noexcept {
	size_t start = m_index;
	advance();  // consume '
	while (is_index_valid() && current().value() != '\'') advance();
	advance();                           // consume '
	std::ignore = consume_identifier();  // just in case it ends with a postfix
	size_t end = m_index;
	return Token{.kind = Token::Kind::CHAR_LITERAL, .span = Span{.start = start, .end = end}};
}

std::optional<Token> Tokenizer::consume_operator() noexcept {
	size_t start = m_index;
	char first_char = current().value();
	advance();  // consume the first operator symbol
	if (is_index_valid()) {
		switch (current().value()) {
		case '|':
		case '&':
			if (first_char == current().value()) advance();
			break;
		}
		if (is_index_valid() && current().value() == '=') {
			advance();
		}
	}
	size_t end = m_index;
	return Token{.kind = Token::Kind::OPERATOR, .span = Span{.start = start, .end = end}};
}

std::optional<Token> Tokenizer::consume_punctuation() noexcept {
	size_t start = m_index;
	char first_char = current().value();
	advance(); // consume the first punctuation symbol
	if (is_index_valid()) {
		switch (current().value()) {
		case ':':
		case '.':
			if (first_char == current().value()) advance();
			break;
		}
	}
	size_t end = m_index;
	return Token{.kind = Token::Kind::PUNCTUATION, .span = Span{.start = start, .end = end}};
}
