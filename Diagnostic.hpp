#pragma once
#include <optional>
#include <string>
#include <vector>

#include "Span.hpp"

class Diagnostic {
public:
	struct Label {
		Span span;
		std::optional<std::string> label;

		Label(Span span, std::optional<std::string> label = {}) : span(span), label(label) {}
	};

	enum class Severity { Warning, Error };

	Diagnostic(Severity severity, std::string title, std::optional<std::string> subtitle = {})
	    : m_severity(severity), m_title(title), m_subtitle(subtitle) {}

	Diagnostic &add_label(Span span, std::optional<std::string> label = {});

	[[nodiscard]] constexpr inline Severity severity() const { return m_severity; }

	void print(std::string const *code) const;

private:
	Severity m_severity;
	std::string m_title;
	std::optional<std::string> m_subtitle;
	std::vector<Label> m_labels;
};
