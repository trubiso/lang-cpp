#include <string_view>

#include "Span.hpp"

struct Token {
	enum class Kind {
		IDENTIFIER,
		NUMBER_LITERAL,
		STRING_LITERAL,
		CHAR_LITERAL,
		OPERATOR,
		PUNCTUATION,
	} kind;

	Span span;

	std::string_view value(std::string_view const &source) const;
};
