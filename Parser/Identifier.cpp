#include "Identifier.hpp"

namespace Parser {

Parser<Identifier, ParserError> identifier() {
	return transform(token_identifier(), [](Token const &identifier) {
		return Identifier{.kind = Identifier::Kind::UNQUALIFIED,
		                  .value = std::get<std::string>(identifier.value)};
	});
};

Parser<Identifier, ParserError> qualified_identifier() {
	auto colon_colon = token_punctuation(Token::Punctuation::COLON_COLON);
	auto inner = optional(colon_colon) & separated_no_trailing(token_identifier(), colon_colon);
	auto filtered = filter(inner,
	                       [](std::tuple<std::optional<Token>, std::vector<Token>> const &data)
	                           -> std::optional<std::string> {
		                       std::vector<Token> const &qualified_path = std::get<1>(data);
		                       // TODO: fix;
		                       if (qualified_path.empty()) return "empty qualified path";
		                       return {};
	                       });
	return transform(
	    filtered,
	    [](std::tuple<std::optional<Token>, std::vector<Token>> const &data) -> Identifier {
		    std::optional<Token> const &absolute_dd = std::get<0>(data);
		    bool is_absolute = absolute_dd.has_value();
		    std::vector<Token> const &qualified_path_tokens = std::get<1>(data);
		    std::vector<std::string> qualified_path{};
		    for (Token const &token : qualified_path_tokens) {
			    qualified_path.push_back(std::get<std::string>(token.value));
		    }
		    if (qualified_path.size() == 1 && !is_absolute)
			    return Identifier{.kind = Identifier::Kind::UNQUALIFIED,
			                      .value = qualified_path.at(0)};
		    Identifier::QualifiedPath qlf{
		        .path = qualified_path,
		        .is_absolute = is_absolute,
		    };
		    return Identifier{.kind = Identifier::Kind::QUALIFIED, .value = qlf};
	    });
}

};  // namespace Parser
