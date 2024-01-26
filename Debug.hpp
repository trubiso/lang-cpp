#include <iostream>

#include "Parser/Expression.hpp"
#include "Parser/Identifier.hpp"
#include "Parser/Statement.hpp"
#include "Parser/Type.hpp"
#include "Tokenizer/Token.hpp"

void debug(Parser::Expression const &expression);
void debug(Parser::Identifier const &identifier);
void debug(Parser::Statement const &statement);
void debug(Parser::Type const &type);
void debug(Token::Operator operator_);

template <typename T> void debug(std::vector<T> const &vector) {
	for (size_t i = 0; i < vector.size(); ++i) {
		debug(vector.at(i));
		if (i != vector.size() - 1) std::cout << ", ";
	}
}
