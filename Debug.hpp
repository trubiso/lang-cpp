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
