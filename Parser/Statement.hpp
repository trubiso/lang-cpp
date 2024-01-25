#include "Identifier.hpp"
#include "Type.hpp"
#include "Expression.hpp"

namespace Parser {

struct Statement {
	// missing: SET, FUNC, RET/YIELD/BREAK, BAREEXPR, CLASS/STRUCT/ENUM/UNION/..., IMPORT, UNSAFE(?)
	enum class Kind { CREATE };

	struct Create {
		Type type;
		Identifier identifier;
		bool mutable_;
		std::optional<Expression> value;
	};

	Kind kind;
	std::variant<Create> value;
};

Parser<Statement, ParserError> statement_create();

Parser<Statement, ParserError> statement();

};  // namespace Parser
