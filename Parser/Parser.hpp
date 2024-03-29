#pragma once
#include <expected>
#include <functional>

#include "../Result.hpp"
#include "../Stream.hpp"
#include "../Tokenizer/Token.hpp"

// thanks to https://github.com/Qqwy/cpp-parser_combinators

namespace Parser {

struct ParserError {
	Span span;
	std::string message;
};

template <typename T, typename E>
struct Parser : public std::function<Result<T, E>(Stream<Token> &)> {
	using std::function<Result<T, E>(Stream<Token> &)>::function;

	typedef T value_type;
	typedef E error_type;
};

Span make_span(Stream<Token> const &input, size_t start_index, size_t end_index);

};  // namespace Parser
