#include "Token.hpp"

std::string Token::value(std::string const &source) const {
	return source.substr(span.start, span.end);
}