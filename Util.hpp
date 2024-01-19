#include <string>
#include <cstdint>

[[nodiscard]] std::string read_file(char const *path) noexcept;

void out_fmt_bold();
void out_fmt_dim();
void out_fmt_italic();
void out_fmt_underline();
void out_fmt_blink();
void out_fmt_reverse();
void out_fmt_hidden();
void out_fmt_strikethrough();
void out_fmt_reset();

void out_fmt_color_fg(uint8_t);
void out_fmt_color_bg(uint8_t);
