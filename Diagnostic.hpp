#include <optional>
#include <string>
#include <vector>

#include "Span.hpp"

class Diagnostic {
public:
	struct Label {
		Span span;
		std::string label;

		Label(Span span, std::string label) : span(span), label(label) {}
	};

	enum class Severity { Warning, Error };

	Diagnostic(Severity severity, std::string title, std::optional<std::string> subtitle = {})
	    : severity(severity), title(title), subtitle(subtitle) {}

	Diagnostic &add_label(Span span, std::string label);

	constexpr inline Severity get_severity() const { return severity; }

	void print(std::string const *code) const;

private:
	Severity severity;
	std::string title;
	std::optional<std::string> subtitle;
	std::vector<Label> labels;
};
