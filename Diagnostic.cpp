#include "Diagnostic.hpp"

#include <iostream>

#include "Util.hpp"

Diagnostic &Diagnostic::add_label(Span span, std::string label) {
	labels.push_back(Label(span, std::move(label)));
	return *this;
}

constexpr uint8_t diagnostic_color(Diagnostic const &diagnostic) noexcept {
	switch (diagnostic.get_severity()) {
	case Diagnostic::Severity::Warning:
		return 220;
	case Diagnostic::Severity::Error:
		return 1;
	default:
		return 0;  // unreachable
	}
}

constexpr char const *severity_name(Diagnostic::Severity severity) noexcept {
	switch (severity) {
	case Diagnostic::Severity::Warning:
		return "warning";
	case Diagnostic::Severity::Error:
		return "error";
	default:
		return "";  // unreachable
	}
}

void Diagnostic::print() const {
	out_fmt_bold();
	out_fmt_color_fg(diagnostic_color(*this));
	std::cout << severity_name(severity) << ":";

	out_fmt_reset();
	std::cout << " ";

	out_fmt_bold();
	std::cout << title << "\n";
	out_fmt_reset();

	if (subtitle.has_value()) {
		out_fmt_italic();
		std::cout << subtitle.value() << "\n";
		out_fmt_reset();
	}

	std::cout << std::endl;
}
