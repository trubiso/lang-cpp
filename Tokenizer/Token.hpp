#pragma once
#include <string>
#include <variant>

#include "../Span.hpp"

struct Token {
	enum class Kind {
		IDENTIFIER,
		NUMBER_LITERAL,
		STRING_LITERAL,
		CHAR_LITERAL,
		OPERATOR,
		PUNCTUATION,
	} kind;

	enum class Keyword {
		LET,
	};

	enum class Operator {
		PLUS,
		PLUS_EQ,
		NEG,
		NEG_EQ,
		STAR,
		STAR_EQ,
		DIV,
		DIV_EQ,
		AMP,
		AMP_EQ,
		AMP_AMP,
		AMP_AMP_EQ,
		BAR,
		BAR_EQ,
		BAR_BAR,
		BAR_BAR_EQ,
		CARET,
		CARET_EQ,
		TILDE,
		EQ,
		EQ_EQ,
		LT,
		LT_EQ,
		GT,
		GT_EQ,
		BANG,
		BANG_EQ,
		QUESTION,
	};

	enum class Punctuation {
		L_PAREN,
		R_PAREN,
		L_BRACKET,
		R_BRACKET,
		L_BRACE,
		R_BRACE,
		DOT,
		DOT_DOT,
		COMMA,
		COLON,
		COLON_COLON,
		SEMICOLON,
		ARROW,
		FAT_ARROW,
	};

	Span span;
	std::variant<std::string, Operator, Punctuation> value;
};

char const *get_variant_name(Token::Kind);
char const *get_variant_name(Token::Keyword);
char const *get_variant_name(Token::Operator);
char const *get_variant_name(Token::Punctuation);
