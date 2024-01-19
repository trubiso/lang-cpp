#include <optional>
#include <string>

#include "Token.hpp"

class Tokenizer {
public:
	Tokenizer(std::string const *source) : m_source(source), m_index(0) {}

	[[nodiscard]] std::optional<Token> next() noexcept;

private:
	[[nodiscard]] constexpr inline std::optional<std::reference_wrapper<char const>> current() const noexcept {
		if (!is_index_valid()) return {};
		return m_source->at(m_index);
	}

	inline bool advance(size_t amount = 1) noexcept {
		m_index += amount;
		return is_index_valid();
	}

	[[nodiscard]] constexpr inline bool is_index_valid() const noexcept { return m_index < m_source->size(); }

	[[nodiscard]] constexpr static inline bool is_whitespace(char x) noexcept {
		return x == ' ' || x == '\n' || x == '\t';
	}

	void consume_whitespace() noexcept;
	[[nodiscard]] std::optional<Token> consume_identifier() noexcept;
	[[nodiscard]] std::optional<Token> consume_number_literal() noexcept;
	[[nodiscard]] std::optional<Token> consume_wrapped_literal(char wrap, Token::Kind kind) noexcept;
	[[nodiscard]] std::optional<Token> consume_operator() noexcept;
	[[nodiscard]] std::optional<Token> consume_punctuation() noexcept;

	std::string const *m_source;
	size_t m_index;
};
