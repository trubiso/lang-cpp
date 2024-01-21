#include "Tokenizer.hpp"

#include <cctype>
#include <tuple>

[[nodiscard]] std::optional<Token> Tokenizer::next() noexcept {
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
	case '~':  // (~= is disallowed)
	case '=':
	case '<':
	case '>':
	case '!':
	case '?':  // (?= is disallowed)
		return consume_operator();
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

[[nodiscard]] std::optional<Token> Tokenizer::consume_identifier() noexcept {
	size_t start = m_index;
	while (is_index_valid() && std::isalnum(current().value())) {
		advance();
	}
	size_t end = m_index;
	Span span = Span{.start = start, .end = end};
	return Token{.kind = Token::Kind::IDENTIFIER, .span = span, .value = span.value(*m_source)};
}

[[nodiscard]] std::optional<Token> Tokenizer::consume_number_literal() noexcept {
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
	Span span = Span{.start = start, .end = end};
	return Token{.kind = Token::Kind::NUMBER_LITERAL, .span = span, .value = span.value(*m_source)};
}

[[nodiscard]] std::optional<Token> Tokenizer::consume_wrapped_literal(char wrap,
                                                                      Token::Kind kind) noexcept {
	size_t start = m_index;
	advance();  // consume wrap character
	while (is_index_valid() && current().value() != wrap) advance();
	if (!is_index_valid()) {
		Diagnostic diagnostic(
		    Diagnostic::Severity::Error,
		    kind == Token::Kind::STRING_LITERAL ? "unclosed string literal"
		                                        : "unclosed character literal",
		    "while trying to find the closing quote, the end of the file was found instead");
		diagnostic.add_label(Span{.start = start, .end = m_index});
		m_diagnostics.push_back(diagnostic);
		return {};
	}
	advance();                           // consume wrap character
	std::ignore = consume_identifier();  // just in case it ends with a postfix
	size_t end = m_index;
	Span span = Span{.start = start, .end = end};
	return Token{.kind = kind, .span = span, .value = span.value(*m_source)};
}

Token::Operator char_to_operator(char c) noexcept {
	using enum Token::Operator;
	switch (c) {
	case '+':
		return PLUS;
	case '-':
		return NEG;
	case '*':
		return STAR;
	case '/':
		return DIV;
	case '&':
		return AMP;
	case '|':
		return BAR;
	case '^':
		return CARET;
	case '~':
		return TILDE;
	case '=':
		return EQ;
	case '<':
		return LT;
	case '>':
		return GT;
	case '!':
		return BANG;
	case '?':
		return QUESTION;
	default:
		[[assume(false)]];
	}
}

Token::Operator add_eq(Token::Operator op) noexcept {
	using enum Token::Operator;
	switch (op) {
	case PLUS:
		return PLUS_EQ;
	case NEG:
		return NEG_EQ;
	case STAR:
		return STAR_EQ;
	case DIV:
		return DIV_EQ;
	case AMP:
		return AMP_EQ;
	case AMP_AMP:
		return AMP_AMP_EQ;
	case BAR:
		return BAR_EQ;
	case BAR_BAR:
		return BAR_BAR_EQ;
	case CARET:
		return CARET_EQ;
	case EQ:
		return EQ_EQ;
	case LT:
		return LT_EQ;
	case GT:
		return GT_EQ;
	case BANG:
		return BANG_EQ;
	default:
		[[assume(false)]];
	}
}

Token::Operator span_to_operator(Span span, std::string const *source) {
	Token::Operator first_char = char_to_operator(source->at(span.start));
	if (span.end <= span.start + 1) {  // <= just in case
		return first_char;
	}
	Token::Operator second_char = char_to_operator(source->at(span.start + 1));
	switch (second_char) {
	case Token::Operator::BAR:
		first_char = Token::Operator::BAR_BAR;
		if (span.start + 2 < span.end) second_char = Token::Operator::EQ;
		break;
	case Token::Operator::AMP:
		first_char = Token::Operator::AMP_AMP;
		if (span.start + 2 < span.end) second_char = Token::Operator::EQ;
		break;
	default:
		break;
	}
	switch (second_char) {
	case Token::Operator::EQ:
		return add_eq(first_char);
	default:
		return first_char;
	}
}

[[nodiscard]] std::optional<Token> Tokenizer::consume_operator() noexcept {
	size_t start = m_index;
	char first_char = current().value();
	advance();  // consume the first operator symbol
	if (is_index_valid()) {
		switch (current().value()) {
		case '|':
		case '&':
			if (first_char == current().value()) advance();
			break;
		case '>':
			if (first_char != '-' && first_char != '=') break;
			advance();  // this is an arrow
			size_t end = m_index;
			Span span = Span{.start = start, .end = end};
			Token::Punctuation value =
			    first_char == '-' ? Token::Punctuation::ARROW : Token::Punctuation::FAT_ARROW;
			return Token{.kind = Token::Kind::PUNCTUATION, .span = span, .value = value};
		}
		// stops ?= or ~= from being tokenized
		if (first_char != '?' && first_char != '~')
			if (is_index_valid() && current().value() == '=') {
				advance();
			}
	}
	size_t end = m_index;
	Span span = Span{.start = start, .end = end};
	Token::Operator value = span_to_operator(span, m_source);
	return Token{.kind = Token::Kind::OPERATOR, .span = span, .value = value};
}

Token::Punctuation char_to_punctuation(char c) noexcept {
	using enum Token::Punctuation;
	switch (c) {
	case '(':
		return L_PAREN;
	case ')':
		return R_PAREN;
	case '[':
		return L_BRACKET;
	case ']':
		return R_BRACKET;
	case '{':
		return L_BRACE;
	case '}':
		return R_BRACE;
	case '.':
		return DOT;
	case ',':
		return COMMA;
	case ':':
		return COLON;
	case ';':
		return SEMICOLON;
	default:
		[[assume(false)]];
	}
}

[[nodiscard]] std::optional<Token> Tokenizer::consume_punctuation() noexcept {
	size_t start = m_index;
	char first_char = current().value();
	Token::Punctuation value = char_to_punctuation(first_char);
	advance();  // consume the first punctuation symbol
	if (is_index_valid()) {
		switch (current().value()) {
		case ':':
		case '.':
			if (first_char == current().value()) {
				if (first_char == ':')
					value = Token::Punctuation::COLON_COLON;
				else
					value = Token::Punctuation::DOT_DOT;
				advance();
			}
			break;
		}
	}
	size_t end = m_index;
	Span span = Span{.start = start, .end = end};
	return Token{.kind = Token::Kind::PUNCTUATION, .span = span, .value = value};
}
