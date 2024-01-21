#include <iostream>

#include "Parser/Combinator.hpp"
#include "Parser/Primitive.hpp"
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
	auto parsed = Parser::token_kind(Token::Kind::IDENTIFIER)(token_stream);
	if (bool(parsed)) {
		auto token = std::get<Token>(parsed);
		std::cout << std::get<std::string>(token.value) << std::endl;
	} else {
		std::cout << "Error" << std::endl;
	}

	std::cout << std::endl;
	return 0;
}
