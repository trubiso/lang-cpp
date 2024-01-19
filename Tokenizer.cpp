#include "Tokenizer.hpp"

#include <cctype>

std::optional<Token> Tokenizer::next() {
	consume_whitespace();
	if (!current().has_value()) return {};
	if (std::isalpha(current().value())) return consume_identifier();
	if (std::isdigit(current().value())) return consume_number_literal();
	switch (current().value()) {
	case '"':
		return consume_wrapped_literal('"', Token::Kind::STRING_LITERAL);
	case '\'':
		return consume_wrapped_literal('\'', Token::Kind::CHAR_LITERAL);
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
	case '?':  // FIXME: disallow ?= (what even is that)
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
	// TODO: be stricter
	size_t start = m_index;
	while (is_index_valid() && (std::isdigit(current().value()) || current().value() == '_'))
		advance();
	if (is_index_valid()) {
		switch (current().value()) {
		case 'x':
		case 'b':
		case 'o':
			advance();  // consume the base character
			break;
		case '.':
			advance();  // consume the decimal point (0x, 0b, 0o are integer only)
			break;
		}
	}
	while (is_index_valid() && (std::isxdigit(current().value()) || current().value() == '_'))
		advance();
	std::ignore = consume_identifier();  // just in case it ends with a postfix
	size_t end = m_index;
	return Token{.kind = Token::Kind::NUMBER_LITERAL, .span = Span{.start = start, .end = end}};
}

std::optional<Token> Tokenizer::consume_wrapped_literal(char wrap, Token::Kind kind) noexcept {
	size_t start = m_index;
	advance();  // consume wrap character
	while (is_index_valid() && current().value() != wrap) advance();
	if (!is_index_valid()) return {};  // if we didn't reach the wrap character, there's no token
	// TODO: log unterminated wrapped literal error
	advance();                           // consume wrap character
	std::ignore = consume_identifier();  // just in case it ends with a postfix
	size_t end = m_index;
	return Token{.kind = kind, .span = Span{.start = start, .end = end}};
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
	advance();  // consume the first punctuation symbol
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
