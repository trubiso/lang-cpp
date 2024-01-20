#include "Combinator.hpp"
#include "Primitive.hpp"

namespace Parser {

template <typename A, typename F>
auto transform(Parser<A> const &parser, F const &function)
    -> Parser<decltype(function(std::declval<A>()))> {
	return [=](Stream<Token> &input) -> Result<decltype(function(std::declval<A>())), ParserError> {
		Result<A, ParserError> result = parser(input);
		if (!bool(result)) return std::get<ParserError>(result);
		return function(std::get<A>(result));
	};
}

template <typename A, typename F>
Parser<A> transform_error(Parser<A> const &parser, F const &function) {
	return [=](Stream<Token> &input) -> Result<A, ParserError> {
		Result<A, ParserError> result = parser(input);
		if (!bool(result)) return ParserError{function(std::get<ParserError>(result))};
		return result;
	};
}

template <typename A, typename B>
auto operator>>(Parser<A> const &a, Parser<B> const &b)
    -> Parser<typename decltype(Result<A, ParserError>() + Result<B, ParserError>())::value_type> {
	return
	    [=](Stream<Token> &input) -> decltype(Result<A, ParserError>() + Result<B, ParserError>()) {
		    Result<A, ParserError> result_a = a(input);
		    if (!bool(result_a)) return std::get<ParserError>(result_a);
		    Result<B, ParserError> result_b = b(input);
		    if (!bool(result_b)) return std::get<ParserError>(result_b);
		    return result_a + result_b;
	    };
}

template <typename A> Parser<A> operator|(Parser<A> const &lhs, Parser<A> const &rhs) {
	return [=](Stream<Token> &input) -> Result<A, ParserError> {
		Result<A, ParserError> result_a = lhs(input);
		if (bool(result_a)) return result_a;
		Result<A, ParserError> result_b = rhs(input);
		if (bool(result_b)) return result_b;

		// TODO: fix
		return ParserError{};
	};
}

template <typename T> Parser<std::optional<T>> optional(Parser<T> const &parser) {
	return parser() | constant(std::nullopt);
}

};
