#include "Debug.hpp"

#include <iostream>

void debug(Parser::Statement const &statement) {
	using enum Parser::Statement::Kind;
	switch (statement.kind) {
	case CREATE: {
		auto const &create = std::get<Parser::Statement::Create>(statement.value);
		std::cout << "CREATE [";
		debug(create.type);
		if (create.mutable_) std::cout << "(mutable) ";
		std::cout << " ";
		debug(create.identifier);
		if (create.value.has_value()) {
			std::cout << " = ";
			debug(create.value.value());
		}
		std::cout << "]";
		break;
	}
	}
}

void debug(Parser::Identifier const &identifier) {
	using enum Parser::Identifier::Kind;
	switch (identifier.kind) {
	case UNQUALIFIED:
		std::cout << std::get<std::string>(identifier.value);
		break;
	case QUALIFIED: {
		auto const &path = std::get<Parser::Identifier::QualifiedPath>(identifier.value);
		if (path.is_absolute) std::cout << "::";
		for (size_t i = 0; i < path.path.size(); ++i) {
			std::cout << path.path.at(i);
			if (i != path.path.size() - 1) std::cout << "::";
		}
		break;
	}
	}
}

void debug(Parser::Type const &type) {
	using enum Parser::Type::Kind;
	std::cout << "[type ";
	switch (type.kind) {
	case IDENTIFIED:
		debug(std::get<Parser::Identifier>(type.value));
		break;
	case BUILT_IN:
		std::cout << "TODO: BUILT_IN";  // TODO: built in
		break;
	case GENERIC: {
		auto const &generic = std::get<Parser::Type::Generic>(type.value);
		debug(*generic.base);
		std::cout << "<";
		for (size_t i = 0; i < generic.generics.size(); ++i) {
			debug(generic.generics.at(i));
			if (i != generic.generics.size() - 1) std::cout << ", ";
		}
		std::cout << ">";
		break;
	}
	case INFERRED:
		std::cout << "inferred";
		break;
	}
	std::cout << "]";
}

void debug(Parser::Expression const &expression) {
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
		debug(*operation.l);
		std::cout << " binaryop";
		debug(operation.operator_);
		std::cout << " ";
		debug(*operation.r);
		std::cout << ")";
	} break;
	case UNARY_OPERATION: {
		auto const &operation = std::get<Parser::Expression::UnaryOperation>(expression.value);
		std::cout << "(unaryop";
		debug(operation.operator_);
		std::cout << " ";
		debug(*operation.value);
		std::cout << ")";
	} break;
	}
}

void debug(Token::Operator operator_) {
	using enum Token::Operator;
	switch (operator_) {
	case PLUS:
		std::cout << "+";
		break;
	case PLUS_EQ:
		std::cout << "+=";
		break;
	case NEG:
		std::cout << "-";
		break;
	case NEG_EQ:
		std::cout << "-=";
		break;
	case STAR:
		std::cout << "*";
		break;
	case STAR_EQ:
		std::cout << "*=";
		break;
	case DIV:
		std::cout << "/";
		break;
	case DIV_EQ:
		std::cout << "/=";
		break;
	case AMP:
		std::cout << "&";
		break;
	case AMP_EQ:
		std::cout << "&=";
		break;
	case AMP_AMP:
		std::cout << "&&";
		break;
	case AMP_AMP_EQ:
		std::cout << "&&=";
		break;
	case BAR:
		std::cout << "|";
		break;
	case BAR_EQ:
		std::cout << "|=";
		break;
	case BAR_BAR:
		std::cout << "||";
		break;
	case BAR_BAR_EQ:
		std::cout << "||=";
		break;
	case CARET:
		std::cout << "^";
		break;
	case CARET_EQ:
		std::cout << "^=";
		break;
	case TILDE:
		std::cout << "~";
		break;
	case EQ:
		std::cout << "=";
		break;
	case EQ_EQ:
		std::cout << "==";
		break;
	case LT:
		std::cout << "<";
		break;
	case LT_EQ:
		std::cout << "<=";
		break;
	case GT:
		std::cout << ">";
		break;
	case GT_EQ:
		std::cout << ">=";
		break;
	case BANG:
		std::cout << "!";
		break;
	case BANG_EQ:
		std::cout << "!=";
		break;
	case QUESTION:
		std::cout << "?";
		break;
	}
}
