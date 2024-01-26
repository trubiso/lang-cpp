#include <iostream>

#include "Debug.hpp"
#include "Parser/Statement.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "Util.hpp"

int main() {
	auto code = read_file("code");
	auto tokenizer = Tokenizer(&code);
	std::vector<Token> tokens{};
	while (true) {
		std::optional<Token> next = tokenizer.next();
		if (!next.has_value()) break;
		tokens.push_back(next.value());
		// std::cout << next.value().value(code) << std::endl;
	}
	for (auto const &diagnostic : tokenizer.diagnostics()) {
		diagnostic.print(&code);
	}
	Stream<Token> token_stream(tokens);
	auto parsed = Parser::statement()(token_stream);
	if (bool(parsed)) {
		auto value = std::get<Parser::Statement>(parsed);
		debug(value);
	} else {
		auto error = std::get<Parser::ParserError>(parsed);
		Diagnostic diag(Diagnostic::Severity::Error, "could not parse", error.message);
		diag.add_label(Span{.start = tokens.at(error.span.start).span.start,
		                    .end = tokens.at(error.span.start).span.end});
		diag.print(&code);
	}

	std::cout << std::endl;
	return 0;
}
