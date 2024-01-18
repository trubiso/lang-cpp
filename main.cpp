#include <iostream>
#include "Util.hpp"
#include "Tokenizer.hpp"

int main() {
	std::cout << "Hello, world!" << std::endl;
	auto code = read_file("code");
	auto tokenizer = Tokenizer(&code);
	std::cout << tokenizer.next().value().span.end;
	return 0;
}
