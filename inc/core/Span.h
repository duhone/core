#pragma once
#include "core/Log.h"

#include <type_traits>

namespace CR::Core {
	// once std::span is available(C++20), get rid of this.
	template<typename T>
	class Span {
	  public:
		Span() = default;
		Span(T* a_data, size_t a_size) : m_data(a_data), m_size(a_size) {}
		~Span() = default;
		Span(const Span<std::remove_const_t<T>>& a_other) : m_data(a_other.data()), m_size(a_other.size()) {}
		Span(const Span<std::add_const_t<T>>& a_other) : m_data(a_other.data()), m_size(a_other.size()) {}
		Span& operator=(const Span&) = default;

		T& operator[](size_t a_index) {
			Log::Assert(a_index < m_size, "span: out of bounds");
			return m_data[a_index];
		}

		const T& operator[](size_t a_index) const {
			Log::Assert(a_index < m_size, "span: out of bounds");
			return m_data[a_index];
		}

		T* data() { return m_data; }

		const T* data() const { return m_data; }

		size_t size() const { return m_size; }

		T* begin() { return m_data; }

		const T* begin() const { return m_data; }

		const T* cbegin() const { return m_data; }

		T* end() { return m_data + m_size; }

		const T* end() const { return m_data + m_size; }

		const T* cend() const { return m_data + m_size; }

	  private:
		T* m_data{nullptr};
		size_t m_size{0};
	};
}    // namespace CR::Core