#include <memory>
#include <variant>
#include <vector>

#include "Identifier.hpp"

namespace Parser {

struct Type {
	enum class Kind { IDENTIFIED, BUILT_IN, GENERIC, INFERRED };

	struct Generic {
		std::unique_ptr<Type> base;
		std::vector<Type> generics;
	};

	struct BuiltIn {
		enum class Kind { INT, UINT, FLOAT, CHAR, STRING, VOID };
		// only int, uint & float have bit width
		std::variant<uint32_t, void> bit_width;
	};

	Kind kind;
	std::variant<Identifier, BuiltIn, Generic, void> value;
};

Parser<Identifier, ParserError> type_identified();
Parser<Type::BuiltIn, ParserError> type_built_in();
Parser<Type::Generic, ParserError> type_generic();
Parser<void, ParserError> type_inferred();

Parser<Type, ParserError> type();

};  // namespace Parser
