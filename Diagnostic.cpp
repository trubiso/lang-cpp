#include "Diagnostic.hpp"

#include <iostream>
#include <tuple>
#include <unordered_map>

#include "Util.hpp"

#define TAB_WIDTH 4

Diagnostic &Diagnostic::add_label(Span span, std::optional<std::string> label) {
	m_labels.push_back(Label(span, std::move(label)));
	return *this;
}

constexpr uint8_t diagnostic_color(Diagnostic const &diagnostic) noexcept {
	switch (diagnostic.severity()) {
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

std::tuple<size_t, size_t> loc(std::string const *code, size_t idx, bool look_for_next = false) {
	size_t l = 1;
	size_t li = 0;
	for (size_t i = 0; i < std::min(idx, code->size()); i++) {
		if (code->at(i) == '\n') {
			l++;
			li = i + 1;  // seems to fix some bugs
		}
	}
	if (look_for_next) {
		size_t old_li = li;
		for (size_t i = idx + 1; i < code->size(); i++) {
			if (code->at(i) == '\n') {
				li = i;
				break;
			}
		}
		if (old_li == li) li = code->size();
	}
	return {l, li};
}

void print_loc_line(size_t loc_pad, std::optional<size_t> loc_curr) {
	out_fmt_color_fg(8);
	if (!loc_curr.has_value())
		std::cout << std::string(loc_pad, ' ');
	else {
		auto size = std::to_string(loc_curr.value()).size();
		if (size <= loc_pad) std::cout << std::string(loc_pad - size, ' ');
		std::cout << loc_curr.value();
	}
	std::cout << " │ ";
}

void print_preline_labels(std::vector<Diagnostic::Label> const &things, std::string const *code,
                          size_t loc_pad, uint8_t color_fg, bool requires_last_padding = true) {
	size_t j;
	for (Diagnostic::Label const &label : things) {
		size_t start_fixed;
		std::tie(std::ignore, start_fixed) = loc(code, label.span.start);

		// print pointy things
		print_loc_line(loc_pad, {});
		out_fmt_color_fg(color_fg);
		j = start_fixed;
		while (j < label.span.start) {
			if (code->at(j) == '\t') {
				std::cout << std::string(TAB_WIDTH, ' ');
				j += TAB_WIDTH;
				continue;
			}
			std::cout << ' ';
			j++;
		}
		while (j < label.span.end) {
			std::cout << '^';
			j++;
		}
		std::cout << "\n";

		// print labels
		print_loc_line(loc_pad, {});
		out_fmt_color_fg(color_fg);
		j = start_fixed;
		while (j < label.span.start) {
			if (code->at(j) == '\t') {
				std::cout << std::string(TAB_WIDTH, ' ');
				j += TAB_WIDTH;
				continue;
			}
			std::cout << ' ';
			j++;
		}
		std::cout << label.label.value() << "\n";

		if (requires_last_padding) {
			print_loc_line(loc_pad, {});
			std::cout << "\n";
		}
	}
}

void print_labels(std::vector<Diagnostic::Label> const &labels, std::string const *code,
                  uint8_t color_fg) {
	size_t start_l = SIZE_MAX;
	size_t end_l = 0;
	std::unordered_map<size_t, std::vector<Diagnostic::Label>> preline_labels;
	for (Diagnostic::Label const &label : labels) {
		if (label.span.start < start_l) start_l = label.span.start;
		if (label.span.end > end_l) end_l = label.span.end;
		if (!label.label.has_value()) continue;
		size_t loc_lab;
		std::tie(loc_lab, std::ignore) = loc(code, label.span.end);
		if (!preline_labels.contains(loc_lab + 1))
			preline_labels.insert({loc_lab + 1, std::vector<Diagnostic::Label>{}});
		preline_labels.at(loc_lab + 1).push_back(label);
	}

	auto [loc_start, start] = loc(code, start_l);
	auto [loc_end, end] = loc(code, end_l - 1, true);
	size_t loc_pad = std::to_string(loc_end).size();
	size_t loc_curr = loc_start;

	std::cout << "\n";
	print_loc_line(loc_pad, {});
	std::cout << "\n";

	for (size_t i = start; i < end; i++) {
		if (i == start || code->at(i) == '\n') {
			if (i != start) std::cout << "\n";
			if (code->at(i) == '\n') loc_curr++;
			std::string loc_curr_str = std::to_string(loc_curr);
			if (preline_labels.contains(loc_curr))
				print_preline_labels(preline_labels.at(loc_curr), code, loc_pad, color_fg);
			print_loc_line(loc_pad, loc_curr);
		}

		if (code->at(i) != '\n') {
			if (code->at(i) == '\t') {
				std::cout << std::string(TAB_WIDTH, ' ');
				continue;
			}
			out_fmt_color_fg(15);
			for (Diagnostic::Label const &label : labels)
				if (i >= label.span.start && i < label.span.end) out_fmt_color_fg(color_fg);
			std::cout << code->at(i);
		}
	}
	std::cout << "\n";
	if (preline_labels.contains(loc_end + 1)) {
		print_preline_labels(preline_labels.at(loc_end + 1), code, loc_pad, color_fg, false);
	}

	print_loc_line(loc_pad, {});
	std::cout << "\n\n";
}

void Diagnostic::print(std::string const *code) const {
	uint8_t color_fg = diagnostic_color(*this);

	// Severity
	out_fmt_bold();
	out_fmt_color_fg(color_fg);
	std::cout << severity_name(m_severity) << ":";
	out_fmt_reset();
	std::cout << " ";

	// Title
	out_fmt_bold();
	std::cout << m_title << "\n";
	out_fmt_reset();

	// Subtitle
	if (m_subtitle.has_value()) {
		out_fmt_italic();
		std::cout << m_subtitle.value() << "\n";
		out_fmt_reset();
	}

	// Labels
	if (m_labels.size() > 0) {
		print_labels(m_labels, code, color_fg);
	}

	std::cout << std::endl;
}
