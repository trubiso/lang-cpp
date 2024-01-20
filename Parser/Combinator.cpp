#include "Combinator.hpp"

#include "Primitive.hpp"

namespace Parser {

template <typename T, typename E, typename F>
    requires std::is_function_v<F>
auto transform(Parser<T, E> const &parser, F const &function)
    -> Parser<decltype(function(std::declval<T>())), E> {
	return [=](Stream<Token> &input) -> Result<decltype(function(std::declval<T>())), E> {
		Result<T, E> result = parser(input);
		if (!bool(result)) return std::get<E>(result);
		return function(std::get<T>(result));
	};
}

template <typename T, typename E, typename F>
    requires std::is_function_v<F>
auto transform_error(Parser<T, E> const &parser, F const &function)
    -> Parser<T, decltype(function(std::declval<E>()))> {
	return [=](Stream<Token> &input) -> Result<T, decltype(function(std::declval<E>()))> {
		Result<T, E> result = parser(input);
		if (!bool(result)) return function(std::get<E>(result));
		return std::get<T>(result);
	};
}

template <typename T1, typename E1, typename T2, typename E2>
auto operator>>(Parser<T1, E1> const &a, Parser<T2, E2> const &b)
    -> Parser<typename decltype(Result<T1, E1>() + Result<T2, E2>())::value_type,
              typename decltype(Result<T1, E1>() + Result<T2, E2>())::error_type> {
	return [=](Stream<Token> &input) -> decltype(Result<T1, E1>() + Result<T2, E2>()) {
		Result<T1, E1> result_a = a(input);
		if (!bool(result_a)) return std::get<E1>(result_a);
		Result<T2, E2> result_b = b(input);
		if (!bool(result_b)) return std::get<E2>(result_b);
		return result_a + result_b;
	};
}

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

template <typename T, typename E>
Parser<std::optional<T>, E> optional(Parser<T, E> const &parser) {
	return parser() | constant<std::optional<T>, E>(std::nullopt);
}

};  // namespace Parser
