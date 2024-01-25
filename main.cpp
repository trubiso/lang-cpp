#include <iostream>

#include "Parser/Combinator.hpp"
#include "Parser/Expression.hpp"
#include "Parser/HighOrderCombinator.hpp"
#include "Parser/Primitive.hpp"
#include "Parser/Type.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "Util.hpp"

void print_expression(Parser::Expression const &expression) {
	using enum Parser::Expression::Kind;
	switch (expression.kind) {
	case CHAR_LITERAL:
		std::cout << "char " << std::get<std::string>(expression.value);
		break;
	case STRING_LITERAL:
		std::cout << "string " << std::get<std::string>(expression.value);
		break;
	case NUMBER_LITERAL:
		std::cout << "number " << std::get<std::string>(expression.value);
		break;
	case IDENTIFIER:
		std::cout << "identifier";
		break;
	case BINARY_OPERATION: {
		auto const &operation = std::get<Parser::Expression::BinaryOperation>(expression.value);
		std::cout << "(";
		print_expression(*operation.l);
		std::cout << " binaryop" << (int)operation.operator_ << " ";
		print_expression(*operation.r);
		std::cout << ")";
	} break;
	case UNARY_OPERATION: {
		auto const &operation = std::get<Parser::Expression::UnaryOperation>(expression.value);
		std::cout << "(unaryop" << (int)operation.operator_ << " ";
		print_expression(*operation.value);
		std::cout << ")";
	} break;
	}
}

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
	auto parsed = Parser::expression()(token_stream);
	if (bool(parsed)) {
		auto expr = std::get<Parser::Expression>(parsed);
		print_expression(expr);
	} else {
		std::cout << "Error" << std::endl;
	}

	std::cout << std::endl;
	return 0;
}
