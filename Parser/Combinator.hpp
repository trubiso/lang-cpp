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
    requires std::is_function_v<F>
auto transform(Parser<T, E> const &parser, F const &function)
    -> Parser<decltype(function(std::declval<T>())), E>;

/// @brief Transforms a parser's error output through the provided function.
/// @tparam T The parser's value type
/// @tparam E The parser's original error type
/// @tparam F The type of the function transforming E to a new value
/// @param parser The parser
/// @param function The function transforming E to a new value
/// @return Returns the transformed parser.
template <typename T, typename E, typename F>
    requires std::is_function_v<F>
auto transform_error(Parser<T, E> const &parser, F const &function)
    -> Parser<T, decltype(function(std::declval<E>()))>;

// (A & B).first
template <typename T1, typename T2, typename E>
Parser<T1, E> operator<<(Parser<T1, E> const &a, Parser<T2, E> const &b);

// (A & B).second
template <typename T1, typename T2, typename E>
Parser<T2, E> operator>>(Parser<T1, E> const &a, Parser<T2, E> const &b);

/// @brief Runs the first parser. If it succeeds, return that value. Otherwise, run the second
/// parser. If it succeeds, return that value. Otherwise, return a combination of both errors.
/// @tparam T Both parsers' value types
/// @tparam E Both parsers' error types
/// @param lhs First parser
/// @param rhs Second parser
/// @return Returns the transformed parser.
template <typename T, typename E>
Parser<T, E> operator|(Parser<T, E> const &lhs, Parser<T, E> const &rhs);

/// @brief Tries to run the parser, and upon failure, returns nothing instead of failing.
/// @tparam T The parser's value type
/// @tparam E The parser's error type
/// @param parser The parser
/// @return Returns the transformed parser.
template <typename T, typename E> Parser<std::optional<T>, E> optional(Parser<T, E> const &parser);

/// @brief Tries to run both parsers sequentially. If any one of them fails, it returns the first
/// error and backtracks. Otherwise, it returns a tuple with the value outputs of both parsers.
/// @tparam T1 First parser's value type
/// @tparam T2 Second parser's value type
/// @tparam E Both parsers' error type
/// @param a First parser
/// @param b Second parser
/// @return Returns the transformed parser.
template <typename T1, typename T2, typename E>
Parser<std::tuple<T1, T2>, E> operator&(Parser<T1, E> const &a, Parser<T2, E> const &b);

};  // namespace Parser
