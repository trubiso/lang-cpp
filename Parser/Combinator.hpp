#pragma once
#include <optional>

#include "Parser.hpp"

namespace Parser {

template <typename A, typename F>
auto transform(Parser<A> const &parser, F const &function)
    -> Parser<decltype(function(std::declval<A>()))>;

template <typename A, typename F>
Parser<A> transform_error(Parser<A> const &parser, F const &function);

template <typename A, typename B>
auto operator>>(Parser<A> const &a, Parser<B> const &b)
    -> Parser<typename decltype(Result<A, ParserError>() + Result<B, ParserError>())::value_type>;

template <typename A> Parser<A> operator|(Parser<A> const &lhs, Parser<A> const &rhs);

template <typename T> Parser<std::optional<T>> optional(Parser<T> const &parser);

};  // namespace Parser
