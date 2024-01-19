#include "Util.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

[[nodiscard]] std::string read_file(char const *path) noexcept {
	std::string content;
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

void out_fmt_bold() { std::cout << "\e[1m"; }
void out_fmt_dim() { std::cout << "\e[2m"; }
void out_fmt_italic() { std::cout << "\e[3m"; }
void out_fmt_underline() { std::cout << "\e[4m"; }
void out_fmt_blink() { std::cout << "\e[5m"; }
void out_fmt_reverse() { std::cout << "\e[7m"; }
void out_fmt_hidden() { std::cout << "\e[8m"; }
void out_fmt_strikethrough() { std::cout << "\e[9m"; }
void out_fmt_reset() { std::cout << "\e[0m"; }

// TODO: make this good
void out_fmt_color_fg(uint8_t c) { std::cout << "\e[38;5;" << (uint32_t)c << "m"; }
void out_fmt_color_bg(uint8_t c) { std::cout << "\e[48;5;" << (uint32_t)c << "m"; }
