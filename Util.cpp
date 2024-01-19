#include "Util.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

[[nodiscard]] std::string_view read_file(char const *path) noexcept {
	std::string_view content;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);
		std::stringstream str;
		str << file.rdbuf();
		file.close();
		content = str.str();
	} catch (std::ifstream::failure const &err) {
		// TODO: proper logging
		std::cout << "[ERROR] Couldn't read file " << path << "\n" << err.what() << std::endl;
	}

	return content;
}
