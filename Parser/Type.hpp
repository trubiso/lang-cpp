#pragma once
#include <variant>
#include <vector>

#include "../Box.hpp"
#include "Identifier.hpp"

namespace Parser {

struct Type {
	enum class Kind { IDENTIFIED, BUILT_IN, GENERIC, INFERRED };

	struct Generic {
		Box<Type> base;
		std::vector<Type> generics;
	};

	struct BuiltIn {
		enum class Kind { INT, UINT, FLOAT, CHAR, STRING, VOID };
		// only int, uint & float have bit width
		std::variant<uint32_t, std::monostate> bit_width;
	};

	Kind kind;
	std::variant<Identifier, BuiltIn, Generic, std::monostate> value;
};

Parser<Type, ParserError> type_identified();
Parser<Type, ParserError> type_built_in();
Parser<Type, ParserError> type_generic();
Parser<Type, ParserError> type_inferred();

Parser<Type, ParserError> type();

};  // namespace Parser
