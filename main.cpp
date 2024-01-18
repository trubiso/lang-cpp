#include <iostream>
#include "Util.hpp"
#include "Tokenizer.hpp"

int main() {
	std::cout << "Hello, world!" << std::endl;
	auto code = read_file("code");
	auto tokenizer = Tokenizer(&code);
	while (true) {
		std::optional<Token> next = tokenizer.next();
		if (!next.has_value()) break;
		std::cout << next.value().value(code) << std::endl;
	}
	return 0;
}
