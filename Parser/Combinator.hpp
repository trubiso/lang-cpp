#pragma once
#include <optional>

#include "Parser.hpp"

namespace Parser {

/// @brief Transforms a parser's value output through the provided function.
/// @tparam T The parser's original value type
/// @tparam E The parser's error type
/// @tparam F The type of the function transforming T to a new value
/// @param parser The parser
/// @param function The function transforming T to a new value
/// @return Returns the new transformed parser.
template <typename T, typename E, typename F>
auto transform(Parser<T, E> const &parser, F const &function)
    -> Parser<decltype(function(std::declval<T>())), E> {
	return [=](Stream<Token> &input) -> Result<decltype(function(std::declval<T>())), E> {
		Result<T, E> result = parser(input);
		if (!bool(result)) return std::get<E>(result);
		return function(std::get<T>(result));
	};
}

/// @brief Transforms a parser's error output through the provided function.
/// @tparam T The parser's value type
/// @tparam E The parser's original error type
/// @tparam F The type of the function transforming E to a new value
/// @param parser The parser
/// @param function The function transforming E to a new value
/// @return Returns the transformed parser.
template <typename T, typename E, typename F>
auto transform_error(Parser<T, E> const &parser, F const &function)
    -> Parser<T, decltype(function(std::declval<E>()))> {
	return [=](Stream<Token> &input) -> Result<T, decltype(function(std::declval<E>()))> {
		Result<T, E> result = parser(input);
		if (!bool(result)) return function(std::get<E>(result));
		return std::get<T>(result);
	};
}

// (A & B).first
template <typename T1, typename T2, typename E>
Parser<T1, E> operator<<(Parser<T1, E> const &a, Parser<T2, E> const &b) {
	return transform(a & b, [](std::tuple<T1, T2> const &value) { return std::get<0>(value); });
}

// (A & B).second
template <typename T1, typename T2, typename E>
Parser<T2, E> operator>>(Parser<T1, E> const &a, Parser<T2, E> const &b) {
	return transform(a & b, [](std::tuple<T1, T2> const &value) { return std::get<1>(value); });
}

/// @brief Runs the first parser. If it succeeds, return that value. Otherwise, run the second
/// parser. If it succeeds, return that value. Otherwise, return a combination of both errors.
/// @tparam T Both parsers' value types
/// @tparam E Both parsers' error types
/// @param lhs First parser
/// @param rhs Second parser
/// @return Returns the transformed parser.
template <typename T, typename E>
Parser<T, E> operator|(Parser<T, E> const &lhs, Parser<T, E> const &rhs) {
	return [=](Stream<Token> &input) -> Result<T, E> {
		Result<T, E> result_a = lhs(input);
		if (bool(result_a)) return result_a;
		Result<T, E> result_b = rhs(input);
		if (bool(result_b)) return result_b;
		return std::get<E>(result_a) + std::get<E>(result_b);
	};
}

/// @brief Tries to run the parser, and upon failure, returns nothing instead of failing.
/// @tparam T The parser's value type
/// @tparam E The parser's error type
/// @param parser The parser
/// @return Returns the transformed parser.
template <typename T, typename E> Parser<std::optional<T>, E> optional(Parser<T, E> const &parser) {
	return parser() | constant<std::optional<T>, E>(std::nullopt);
}

/// @brief Tries to run both parsers sequentially. If any one of them fails, it returns the first
/// error and backtracks. Otherwise, it returns a tuple with the value outputs of both parsers.
/// @tparam T1 First parser's value type
/// @tparam T2 Second parser's value type
/// @tparam E Both parsers' error type
/// @param a First parser
/// @param b Second parser
/// @return Returns the transformed parser.
template <typename T1, typename T2, typename E>
Parser<std::tuple<T1, T2>, E> operator&(Parser<T1, E> const &a, Parser<T2, E> const &b) {
	return [=](Stream<Token> &input) -> Result<std::tuple<T1, T2>, E> {
		size_t original_index = input.index();
		Result<T1, E> result_a = a(input);
		if (!bool(result_a)) {
			input.set_index(original_index);
			return std::get<E>(result_a);
		}
		Result<T2, E> result_b = b(input);
		if (!bool(result_b)) {
			input.set_index(original_index);
			return std::get<E>(result_b);
		}
		return std::tie(std::get<T1>(result_a), std::get<T2>(result_b));
	};
}

};  // namespace Parser
