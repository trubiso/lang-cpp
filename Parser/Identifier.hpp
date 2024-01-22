#include <optional>
#include <string>
#include <vector>
#include <algorithm>

#include "HighOrderCombinator.hpp"
#include "Primitive.hpp"

namespace Parser {

struct Identifier {
	enum class Kind {
		UNQUALIFIED,
		QUALIFIED,
	};

	struct QualifiedPath {
		std::vector<std::string> path;
		bool is_absolute = false;
	};

	Kind kind;
	std::variant<std::string, QualifiedPath> value;
};

Parser<Identifier, ParserError> identifier();
Parser<Identifier, ParserError> qualified_identifier();

};  // namespace Parser
