#include "Type.hpp"

namespace Parser {

Parser<Identifier, ParserError> type_identified() { return qualified_identifier(); }

Parser<Type::BuiltIn, ParserError> type_built_in() {
	// TODO: do
	return [](...) { return ParserError{}; };
};

Parser<Type::Generic, ParserError> type_generic() {
	return transform(type() & angled(separated_by_comma(type())),
	                 [](std::tuple<Type, std::vector<Type>> const &data) {
		                 Type const &base = std::get<0>(data);
		                 std::vector<Type> const &generics = std::get<1>(data);
		                 auto value = Type::Generic{
		                     .base = std::make_unique<Type>(base),
		                     .generics = generics,
		                 };
		                 return Type{
		                     .kind = Type::Kind::GENERIC,
		                     .value = value,
		                 };
	                 });
}
Parser<void, ParserError> type_inferred();

Parser<Type, ParserError> type();

};  // namespace Parser
