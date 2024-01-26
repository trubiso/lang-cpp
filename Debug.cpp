#include "Debug.hpp"

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
		debug(std::get<Parser::Identifier>(expression.value));
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
	case CALL: {
		auto const &call = std::get<Parser::Expression::Call>(expression.value);
		std::cout << "[call (";
		debug(*call.callee);
		std::cout << ") with generics <";
		debug(call.generics);
		std::cout << "> and arguments (";
		debug(call.arguments);
		std::cout << ")]";
	} break;
	}
}

void debug(Token::Operator operator_) {
	std::cout << get_variant_name(operator_);
}
