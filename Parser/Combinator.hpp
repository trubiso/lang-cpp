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

// Filter function returns nothing if the item passes, otherwise returns an error
template <typename T, typename E>
Parser<T, E> filter(Parser<T, E> const &parser,
                    std::function<std::optional<E>(T const &)> const &function) {
	return [=](Stream<Token> &input) -> Result<T, E> {
		size_t original_index = input.index();
		Result<T, E> result = parser(input);
		if (!bool(result)) return std::get<E>(result);
		std::optional<E> transformed = function(std::get<T>(result));
		if (!transformed.has_value()) return std::get<T>(result);
		input.set_index(original_index);
		return transformed.value();
	};
}

// parses the parser as many times as it can
template <typename T, typename E> Parser<std::vector<T>, void> many(Parser<T, E> const &parser) {
	return [=](Stream<Token> &input) -> Result<std::vector<T>, void> {
		std::vector<T> elements{};
		while (true) {
			Result<T, E> element = parser(input);
			if (!bool(element)) return elements;
			elements.push_back(std::get<T>(element));
		}
	};
}

// many with size constraint
template <typename T, typename E>
Parser<std::vector<T>, E> at_least(Parser<T, E> const &parser, size_t quantity, E const &error) {
	return filter(many(parser), [=](std::vector<T> const &element) {
		if (element.size() >= quantity) return {};
		return error;
	});
}

// [parser] [separator] [parser] [...] [separator] [parser] ([separator])
// (allows trailing)
template <typename T1, typename T2, typename E1, typename E2, typename EO>
Parser<std::vector<T1>, EO> separated(Parser<T1, E1> const &parser,
                                      Parser<T2, E2> const &separator) {
	return [=](Stream<Token> &input) -> Result<std::vector<T1>, EO> {
		std::vector<T1> elements{};
		while (true) {
			Result<T1, E1> element = parser(input);
			if (!bool(element)) return elements;
			elements.push_back(std::get<T1>(element));
			Result<T2, E2> separate = separator(input);
			if (!bool(separate)) return elements;
		}
	};
}

template <typename T1, typename T2, typename E>
inline Parser<std::vector<T1>, E> separated(Parser<T1, E> const &parser,
                                            Parser<T2, E> const &separator) {
	return separated<T1, T2, E, E, E>(parser, separator);
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
		// TODO: fix
		return ParserError{};
		// return std::get<E>(result_a) + std::get<E>(result_b);
	};
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
