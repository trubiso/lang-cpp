#include "Diagnostic.hpp"

#include <iostream>
#include <tuple>
#include <unordered_map>

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

std::tuple<size_t, size_t> loc(std::string const *code, size_t idx, bool look_for_next = false) {
	size_t l = 1;
	size_t li = 0;
	for (size_t i = 0; i <= idx; i++) {
		if (code->at(i) == '\n') {
			l++;
			li = i;
		}
	}
	if (look_for_next) {
		size_t old_li = li;
		for (size_t i = idx + 1; i < code->size(); i++) {
			if (code->at(i) == '\n') {
				li = i;
			}
		}
		if (old_li == li) li = code->size();
	}
	return {l, li};
}

void print_preline_labels(std::vector<Diagnostic::Label> const &things, std::string const *code, size_t loc_pad, uint8_t color_fg) {
	size_t j;
	for (Diagnostic::Label const &label : things) {
		size_t start_fixed;
		std::tie(std::ignore, start_fixed) = loc(code, label.span.start);
		out_fmt_color_fg(8);
		std::cout << std::string(loc_pad, ' ') << " │ " << std::string();
		out_fmt_color_fg(color_fg);
		j = start_fixed;
		while (j < label.span.start) {
			std::cout << ' ';
			j++;
		}
		while (j < label.span.end) {
			std::cout << '^';
			j++;
		}
		std::cout << "\n";

		out_fmt_color_fg(8);
		std::cout << std::string(loc_pad, ' ') << " │ " << std::string();
		out_fmt_color_fg(color_fg);
		j = start_fixed;
		while (j < label.span.start) {
			std::cout << ' ';
			j++;
		}
		std::cout << label.label << "\n";

		out_fmt_color_fg(8);
		std::cout << std::string(loc_pad, ' ') << " │ " << std::string();
		std::cout << "\n";
	}
}

void Diagnostic::print(std::string const *code) const {
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

	if (labels.size() > 0) {
		std::cout << "\n";
		size_t start_l = SIZE_MAX;
		size_t end_l = 0;
		std::unordered_map<size_t, std::vector<Label>> preline_labels;
		for (Label const &label : labels) {
			if (label.span.start < start_l) start_l = label.span.start;
			if (label.span.end > end_l) end_l = label.span.end;
			size_t loc_lab;
			std::tie(loc_lab, std::ignore) = loc(code, label.span.end);
			if (!preline_labels.contains(loc_lab + 1))
				preline_labels.insert({loc_lab + 1, std::vector<Label>{}});
			preline_labels.at(loc_lab + 1).push_back(label);
		}

		auto [loc_start, start] = loc(code, start_l);
		auto [loc_end, end] = loc(code, end_l - 1, true);
		size_t loc_pad = std::to_string(loc_end).size();
		size_t loc_curr = loc_start;

		for (size_t i = start; i < end; i++) {
			if (i == start || code->at(i) == '\n') {
				if (i != start) std::cout << "\n";
				if (code->at(i) == '\n') loc_curr++;
				std::string loc_curr_str = std::to_string(loc_curr);
				if (preline_labels.contains(loc_curr)) {
					// TODO: sort ltr
					print_preline_labels(preline_labels.at(loc_curr), code, loc_pad, diagnostic_color(*this));
				}
				out_fmt_color_fg(8);
				std::cout << std::string(loc_pad - loc_curr_str.size(), ' ') << loc_curr << " │ ";
			}
			
			if (code->at(i) != '\n') {
				out_fmt_color_fg(15);
				for (Label const &label : labels)
					if (i >= label.span.start && i < label.span.end)
						out_fmt_color_fg(diagnostic_color(*this));
				std::cout << code->at(i);
			}
		}
		if (preline_labels.contains(loc_end + 1)) {
			std::cout << "\n";
			// TODO: why is this off by 1
			print_preline_labels(preline_labels.at(loc_end + 1), code, loc_pad, diagnostic_color(*this));
		}

		std::cout << "\n";
	}

	std::cout << std::endl;
}
