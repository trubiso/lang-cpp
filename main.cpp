#include <iostream>

#include "Diagnostic.hpp"
#include "Tokenizer.hpp"
#include "Util.hpp"

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
	diag1.add_label(Span{.start = 11, .end = 28}, "oh no!")
	    .add_label(Span{.start = 1, .end = 6}, "he")
	    .add_label(Span{.start = 8, .end = 9}, "hee")
	    .add_label(Span{.start = 11, .end = 26}, "your string's ugly")
	    .add_label(Span{.start = code.size() - 3, .end = code.size() - 2}, "eugh");
	diag1.print(&code);
	Diagnostic diag2(Diagnostic::Severity::Error, "that was unfortunate");
	diag2.print(&code);
	return 0;
}
