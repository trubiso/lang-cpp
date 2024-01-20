#pragma once
#include <optional>

#include "Parser.hpp"

namespace Parser {

template <typename T, typename E, typename F>
    requires std::is_function_v<F>
auto transform(Parser<T, E> const &parser, F const &function)
    -> Parser<decltype(function(std::declval<T>())), E>;

template <typename T, typename E, typename F>
    requires std::is_function_v<F>
auto transform_error(Parser<T, E> const &parser, F const &function)
    -> Parser<T, decltype(function(std::declval<E>()))>;

template <typename T1, typename E1, typename T2, typename E2>
auto operator>>(Parser<T1, E1> const &a, Parser<T2, E2> const &b)
    -> Parser<typename decltype(Result<T1, E1>() + Result<T2, E2>())::value_type,
              typename decltype(Result<T1, E1>() + Result<T2, E2>())::error_type>;

template <typename T, typename E>
Parser<T, E> operator|(Parser<T, E> const &lhs, Parser<T, E> const &rhs);

template <typename T, typename E>
Parser<std::optional<T>, E> optional(Parser<T, E> const &parser);

};  // namespace Parser
