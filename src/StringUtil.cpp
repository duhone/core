#include "core/StringUtil.h"

bool CR::Core::CaseInsensitiveCompare(const std::wstring& s1, const std::wstring& s2) {
	if(s1.length() != s2.length()) return false;
	auto iter1   = s1.cbegin();
	auto iter2   = s2.cbegin();
	auto endIter = s1.cend();

	while(iter1 != endIter) {
		wchar_t c1 = *iter1;
		wchar_t c2 = *iter2;
		if(c1 >= 'A' && c1 <= 'Z') c1 += 'a' - 'A';
		if(c2 >= 'A' && c2 <= 'Z') c2 += 'a' - 'A';

		if(c1 != c2) return false;
		++iter1;
		++iter2;
	}
	return true;
}