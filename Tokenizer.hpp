#include <optional>
#include <string>

#include "Token.hpp"

class Tokenizer {
public:
	Tokenizer(std::string const *source) : m_source(source), m_index(0) {}

	std::optional<Token> next();

private:
	constexpr inline std::optional<std::reference_wrapper<char const>> current() const noexcept {
		if (!is_index_valid()) return {};
		return m_source->at(m_index);
	}

	inline bool advance(size_t amount = 1) noexcept {
		m_index += amount;
		return is_index_valid();
	}

	constexpr inline bool is_index_valid() const noexcept { return m_index < m_source->size(); }

	void consume_whitespace() noexcept;
	Token consume_identifier() noexcept;

	std::string const *m_source;
	size_t m_index;
};
