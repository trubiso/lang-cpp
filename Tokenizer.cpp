#include "Tokenizer.hpp"

#include <cctype>

void Tokenizer::consume_whitespace() {
	while (std::isblank(current())) {
		advance();
	}
}
