#include "Token.hpp"

std::string_view Token::value(std::string_view const &source) const {
	return source.substr(span.start, span.end - span.start);
}