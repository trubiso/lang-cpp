#include "Token.hpp"

char const *get_variant_name(Token::Kind kind) {
	using enum Token::Kind;
	switch (kind) {
	case IDENTIFIER:
		return "identifier";
	case NUMBER_LITERAL:
		return "number literal";
	case STRING_LITERAL:
		return "string literal";
	case CHAR_LITERAL:
		return "char literal";
	case OPERATOR:
		return "operator";
	case PUNCTUATION:
		return "punctuation";
	}
}

char const *get_variant_name(Token::Keyword keyword) { return "TODO: operator variant name"; }

char const *get_variant_name(Token::Operator operator_) {
	using enum Token::Operator;
	switch (operator_) {
	case PLUS:
		return "+";
	case PLUS_EQ:
		return "+=";
	case NEG:
		return "-";
	case NEG_EQ:
		return "-=";
	case STAR:
		return "*";
	case STAR_EQ:
		return "*=";
	case DIV:
		return "/";
	case DIV_EQ:
		return "/=";
	case AMP:
		return "&";
	case AMP_EQ:
		return "&=";
	case AMP_AMP:
		return "&&";
	case AMP_AMP_EQ:
		return "&&=";
	case BAR:
		return "|";
	case BAR_EQ:
		return "|=";
	case BAR_BAR:
		return "||";
	case BAR_BAR_EQ:
		return "||=";
	case CARET:
		return "^";
	case CARET_EQ:
		return "^=";
	case TILDE:
		return "~";
	case EQ:
		return "=";
	case EQ_EQ:
		return "==";
	case LT:
		return "<";
	case LT_EQ:
		return "<=";
	case GT:
		return ">";
	case GT_EQ:
		return ">=";
	case BANG:
		return "!";
	case BANG_EQ:
		return "!=";
	case QUESTION:
		return "?";
	}
}

char const *get_variant_name(Token::Punctuation punctuation) {
	using enum Token::Punctuation;
	switch (punctuation) {
	case L_PAREN:
		return "(";
	case R_PAREN:
		return ")";
	case L_BRACKET:
		return "[";
	case R_BRACKET:
		return "]";
	case L_BRACE:
		return "{";
	case R_BRACE:
		return "}";
	case DOT:
		return ".";
	case DOT_DOT:
		return "..";
	case COMMA:
		return ",";
	case COLON:
		return ":";
	case COLON_COLON:
		return "::";
	case SEMICOLON:
		return ";";
	case ARROW:
		return "->";
	case FAT_ARROW:
		return "=>";
	}
}
