#include <iostream>
#include "Util.hpp"
#include "Tokenizer.hpp"
#include "Diagnostic.hpp"

int main() {
	std::cout << "Hello, world!" << std::endl;
	auto code = read_file("code");
	auto tokenizer = Tokenizer(&code);
	while (true) {
		std::optional<Token> next = tokenizer.next();
		if (!next.has_value()) break;
		std::cout << next.value().value(code) << std::endl;
	}
	Diagnostic diag1(Diagnostic::Severity::Warning, "i am warning you!!", "just so you know");
	diag1.add_label(Span {.start = 128, .end = code.size() - 6}, "eugh");
	diag1.print(&code);
	Diagnostic diag2(Diagnostic::Severity::Error, "that was unfortunate");
	diag2.print(&code);
	return 0;
}
