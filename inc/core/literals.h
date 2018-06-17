#pragma once

namespace CR::Core { namespace Literals {
	constexpr unsigned long long operator"" _Kb(unsigned long long a_bytes) { return a_bytes * 1024; }
	constexpr unsigned long long operator"" _Mb(unsigned long long a_bytes) { return a_bytes * 1024 * 1024; }
	constexpr unsigned long long operator"" _Gb(unsigned long long a_bytes) { return a_bytes * 1024 * 1024 * 1024; }
}}    // namespace CR::Core::Literals