#include <optional>
#include <string>

#include "Token.hpp"

class Tokenizer {
	std::optional<Token> next();

private:
	constexpr inline char const &current() const { return m_source->at(m_index); }
	inline bool advance(size_t amount = 1) {
		if (m_index - 1 >= m_source->size() && amount > 0)
			return false;
		else
			m_index += amount;
		return true;
	}

	void consume_whitespace();

	std::string const *m_source;
	size_t m_index;
};
