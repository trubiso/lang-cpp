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
		std::cout << "Error" << std::endl;
	}

	std::cout << std::endl;
	return 0;
}
