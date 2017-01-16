#pragma once
#include<string>
#include "algorithm.h"

namespace CR
{
	namespace Core
	{
		struct UnicodeToAscii
		{
			char operator()(wchar_t _char)
			{
				int numConverted;
				char result;
				wctomb_s(&numConverted, &result, 1, _char);
				return result;
			}
		};
		struct AsciiToUnicode
		{
			wchar_t operator()(char _char)
			{
				return static_cast<wchar_t>(_char);
			}
		};


		struct Convert
		{
			std::string operator()(const std::wstring &_original)
			{
				std::string result;
				result.resize(_original.size());
				transform(_original, result, UnicodeToAscii());
				return result;
			}
			std::wstring operator()(const std::string &_original)
			{
				std::wstring result;
				result.resize(_original.size());
				transform(_original, result, AsciiToUnicode());
				return result;
			}
		};

		bool CaseInsensitiveCompare(const std::wstring &s1, const std::wstring &s2);

	}
}

