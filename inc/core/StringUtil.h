#pragma once
#include "core/algorithm.h"
#include <string>

namespace CR::Core {
	struct UnicodeToAscii {
		char operator()(wchar_t _char) {
			int numConverted;
			char result;
			wctomb_s(&numConverted, &result, 1, _char);
			return result;
		}
	};
	struct AsciiToUnicode {
		wchar_t operator()(char _char) { return static_cast<wchar_t>(_char); }
	};

	[[nodiscard]] std::string Convert(const std::wstring& _original) {
		std::string result;
		result.resize(_original.size());
		transform(_original, result, UnicodeToAscii());
		return result;
	}

	[[nodiscard]] std::wstring Convert(const std::string& _original) {
		std::wstring result;
		result.resize(_original.size());
		transform(_original, result, AsciiToUnicode());
		return result;
	}

	[[nodiscard]] bool CaseInsensitiveCompare(const std::wstring& s1, const std::wstring& s2);
}    // namespace CR::Core
