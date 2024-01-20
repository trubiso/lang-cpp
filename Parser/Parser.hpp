#pragma once
#include <expected>
#include <functional>

#include "../Result.hpp"
#include "../Stream.hpp"
#include "../Tokenizer/Token.hpp"

// thanks to https://github.com/Qqwy/cpp-parser_combinators

namespace Parser {

struct ParserError {};

template <typename T, typename E>
struct Parser : public std::function<Result<T, E>(Stream<Token> &)> {
	using std::function<Result<T, E>(Stream<Token> &)>::function;

	typedef T value_type;
	typedef E error_type;
};

};
