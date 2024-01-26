#include "Parser.hpp"

namespace Parser {

Span make_span(Stream<Token> const &input, size_t start_index, size_t end_index) {
	std::optional<Token> start = input.at(start_index);
	std::optional<Token> end = input.at(end_index);
	return Span{.start = start.has_value() ? start.value().span.start : input.last().span.start,
	            .end = end.has_value() ? end.value().span.end : input.last().span.end};
}

};  // namespace Parser
