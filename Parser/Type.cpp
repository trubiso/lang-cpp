#include "Type.hpp"

namespace Parser {

Parser<Type, ParserError> type_identified() {
	return transform(qualified_identifier(), [](Identifier const &identifier) {
		return Type{.kind = Type::Kind::IDENTIFIED, .value = identifier};
	});
}

Parser<Type, ParserError> type_built_in() {
	// TODO: do
	return [](...) { return ParserError{}; };
};

// TODO: fix this so it can be used. use a mechanism similar to expressions where the other 3 are
// atoms
Parser<Type, ParserError> type_generic() {
	auto inner = lazy(type()) & angled(separated_by_comma(lazy(type())));
	return transform(inner, [](std::tuple<Type, std::vector<Type>> const &data) {
		Type const &base = std::get<0>(data);
		std::vector<Type> const &generics = std::get<1>(data);
		return Type{
		    .kind = Type::Kind::GENERIC,
		    .value = (decltype(Type::value))(Type::Generic{
		        .base = base,
		        .generics = generics,
		    }),
		};
	});
}
Parser<Type, ParserError> type_inferred() {
	auto filtered = filter(
	    qualified_identifier(), [](Identifier const &identifier) -> std::optional<std::string> {
		    if (identifier.kind != Identifier::Kind::UNQUALIFIED)
			    return "couldn't find unqualified identifier while searching for a discarded type";
		    if (std::get<std::string>(identifier.value) != "_")
			    return "incorrect name for discarded type";
		    return {};
	    });
	return transform(filtered, [](Identifier const &) {
		return Type{.kind = Type::Kind::INFERRED, .value = std::monostate{}};
	});
}

Parser<Type, ParserError> type() { return type_inferred() | type_built_in() | type_identified(); }

};  // namespace Parser
