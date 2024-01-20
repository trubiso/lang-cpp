#include <vector>

template <typename T>
class Stream {
public:
	Stream(std::vector<T> data) : m_data(data), m_index(0) {}

	constexpr inline T const &peek() const { return m_data.at(m_index); }
	inline void ignore() { m_index++; }

private:
	std::vector<T> m_data;
	size_t m_index;
};
