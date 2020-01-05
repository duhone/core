#pragma once

#include <core/Span.h>

#include <cstddef>

namespace CR::Core {
	// Paul Hsieh hash
	uint32_t HashFast(Span<std::byte> a_data);
}    // namespace CR::Core