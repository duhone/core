#pragma once
#include "core/Log.h"

#include <type_traits>

namespace CR::Core {
	// once std::span is available(C++20), get rid of this.
	template<typename T>
	class Span final {
	  public:
		Span() = default;
		template<size_t N>
		Span(T (&a_data)[N]) : m_data(a_data), m_size(N) {}
		Span(T* a_data, size_t a_size) : m_data(a_data), m_size(a_size) {}
		~Span()               = default;
		Span(const Span&)     = default;
		Span(Span&&) noexcept = default;
		Span& operator=(const Span&) = default;
		Span& operator=(Span&&) noexcept = default;

		T& operator[](size_t a_index) {
			Log::Assert(a_index < m_size, "span: out of bounds");
			return m_data[a_index];
		}

		const T& operator[](size_t a_index) const {
			Log::Assert(a_index < m_size, "span: out of bounds");
			return m_data[a_index];
		}

		[[nodiscard]] T* data() { return m_data; }

		[[nodiscard]] const T* data() const { return m_data; }

		[[nodiscard]] size_t size() const { return m_size; }

		[[nodiscard]] T* begin() { return m_data; }

		[[nodiscard]] const T* begin() const { return m_data; }

		[[nodiscard]] const T* cbegin() const { return m_data; }

		[[nodiscard]] T* end() { return m_data + m_size; }

		[[nodiscard]] const T* end() const { return m_data + m_size; }

		[[nodiscard]] const T* cend() const { return m_data + m_size; }

	  private:
		T* m_data{nullptr};
		size_t m_size{0};
	};
}    // namespace CR::Core
