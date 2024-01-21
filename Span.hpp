#pragma once
#include <cstddef>
#include <string>

struct Span {
	size_t start;
	size_t end;

	inline constexpr std::string value(std::string const &source) const {
		return source.substr(start, end - start);
	}
};