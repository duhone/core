﻿#pragma once

#include "core/Log.h"

#include <memory>
#include <memory_resource>

namespace CR::Core {
	// from P1072R1 looks like this isn't going to make it into standard c++.
	// Still following standard proposal though just in case, if it doesn make it in, easy to swap this out.
	// This is slightly simplified from standard proposal. Doesn't support string/vector interop.
	// Also doesn't support pmr allocators at this time.
	template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
	class storage_buffer {
	  public:
		// types
		using value_type             = T;
		using allocator_type         = Allocator;
		using pointer                = T*;
		using const_pointer          = const T*;
		using reference              = T&;
		using const_reference        = const T&;
		using size_type              = std::size_t;
		using iterator               = T*;
		using const_iterator         = const T*;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// constructors/destructors
		storage_buffer() noexcept = default;
		storage_buffer(const Allocator& alloc) noexcept;
		storage_buffer(storage_buffer<T, Allocator>&& s) noexcept;
		~storage_buffer();
		allocator_type get_allocator() const noexcept;

		// assignment
		storage_buffer& operator=(storage_buffer<T, Allocator>&& s) noexcept;
		// storage_buffer& operator=(basic_string<T, char_traits<T>, Allocator>&& s);
		// storage_buffer& operator=(vector<T, Allocator>&& v);

		// iterators
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		// capacity
		[[nodiscard]] bool empty() const noexcept;
		[[nodiscard]] size_type size() const noexcept;
		[[nodiscard]] size_type max_size() const noexcept;
		[[nodiscard]] size_type capacity() const noexcept;

		void prepare(size_type n);
		void commit(size_type n);

		// element access
		reference operator[](size_type n);
		const_reference operator[](size_type n) const;
		// reference at(size_type n);
		// const_reference at(size_type n) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;

		// data access
		pointer data() noexcept;
		const_pointer data() const noexcept;

		// modifiers
		// void swap(storage_buffer&) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
		//                                    std::allocator_traits<Allocator>::is_always_equal::value);

		// Disable copy
		storage_buffer(const storage_buffer&) = delete;
		storage_buffer& operator=(const storage_buffer&) = delete;

	  private:
		T* m_data{nullptr};
		size_t m_size{0};
		size_t m_capacity{0};
		Allocator m_allocator;
	};

	template<typename T, typename Allocator>
	inline storage_buffer<T, Allocator>::storage_buffer(const Allocator& alloc) noexcept : m_allocator(alloc) {}

	template<typename T, typename Allocator>
	inline storage_buffer<T, Allocator>::storage_buffer(storage_buffer<T, Allocator>&& s) noexcept {
		*this = std::move(s);
	}

	template<typename T, typename Allocator>
	inline storage_buffer<T, Allocator>&
	    storage_buffer<T, Allocator>::operator=(storage_buffer<T, Allocator>&& s) noexcept {
		if(m_data) { m_allocator.deallocate(m_data, m_capacity); }
		m_data     = s.m_data;
		m_size     = s.m_size;
		m_capacity = s.m_capacity;

		s.m_data     = nullptr;
		s.m_size     = 0;
		s.m_capacity = 0;

		return *this;
	}

	template<typename T, typename Allocator>
	inline storage_buffer<T, Allocator>::~storage_buffer() {
		if(m_data) { m_allocator.deallocate(m_data, m_capacity); }
	}

	template<typename T, typename Allocator>
	inline Allocator storage_buffer<T, Allocator>::get_allocator() const noexcept {
		return m_allocator;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::iterator storage_buffer<T, Allocator>::begin() noexcept {
		return m_data;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_iterator storage_buffer<T, Allocator>::begin() const noexcept {
		return m_data;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::iterator storage_buffer<T, Allocator>::end() noexcept {
		return m_data + m_size;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_iterator storage_buffer<T, Allocator>::end() const noexcept {
		return m_data + m_size;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::reverse_iterator storage_buffer<T, Allocator>::rbegin() noexcept {
		return std::make_reverse_iterator(end());
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reverse_iterator storage_buffer<T, Allocator>::rbegin() const
	    noexcept {
		return std::make_reverse_iterator(end());
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::reverse_iterator storage_buffer<T, Allocator>::rend() noexcept {
		return std::make_reverse_iterator(begin());
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reverse_iterator storage_buffer<T, Allocator>::rend() const
	    noexcept {
		return std::make_reverse_iterator(begin());
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_iterator storage_buffer<T, Allocator>::cbegin() const noexcept {
		return begin();
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_iterator storage_buffer<T, Allocator>::cend() const noexcept {
		return end();
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reverse_iterator storage_buffer<T, Allocator>::crbegin() const
	    noexcept {
		return rbegin();
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reverse_iterator storage_buffer<T, Allocator>::crend() const
	    noexcept {
		return rend();
	}

	template<typename T, typename Allocator>
	inline bool storage_buffer<T, Allocator>::empty() const noexcept {
		return m_size == 0;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::size_type storage_buffer<T, Allocator>::size() const noexcept {
		return m_size;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::size_type storage_buffer<T, Allocator>::max_size() const noexcept {
		return std::numeric_limits<size_type>::max();
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::size_type storage_buffer<T, Allocator>::capacity() const noexcept {
		return m_capacity;
	}

	template<typename T, typename Allocator>
	inline void storage_buffer<T, Allocator>::prepare(size_type n) {
		if(n > m_capacity) {
			T* newData = m_allocator.allocate(n);
			if constexpr(std::is_trivially_copyable_v<T>) {
				memcpy(newData, m_data, m_size * sizeof(T));
			} else if constexpr(std::is_move_assignable_v<T>) {
				for(size_type i = 0; i < m_size; ++i) { newData[i] = std::move(m_data[i]); }
			} else {
				for(size_type i = 0; i < m_size; ++i) { newData[i] = m_data[i]; }
			}
			m_allocator.deallocate(m_data, m_capacity);
			m_data     = newData;
			m_capacity = n;
		}
	}

	template<typename T, typename Allocator>
	inline void storage_buffer<T, Allocator>::commit(size_type n) {
		Log::Assert(n <= m_capacity, "Tried to commit a range that wasn't prepared");
		m_size = std::max(m_size, n);
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::reference storage_buffer<T, Allocator>::operator[](size_type n) {
		Log::Assert(n < m_capacity, "tried to access element that doesnt exist");
		return m_data[n];
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reference
	    storage_buffer<T, Allocator>::operator[](size_type n) const {
		Log::Assert(n < m_capacity, "tried to access element that doesnt exist");
		return m_data[n];
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::reference storage_buffer<T, Allocator>::front() {
		Log::Assert(m_size > 0, "tried to get front element on an empty buffer");
		return m_data[0];
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reference storage_buffer<T, Allocator>::front() const {
		Log::Assert(m_size > 0, "tried to get front element on an empty buffer");
		return m_data[0];
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::reference storage_buffer<T, Allocator>::back() {
		Log::Assert(m_size > 0, "tried to get back element on an empty buffer");
		return m_data[m_size - 1];
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_reference storage_buffer<T, Allocator>::back() const {
		Log::Assert(m_size > 0, "tried to get back element on an empty buffer");
		return m_data[m_size - 1];
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::pointer storage_buffer<T, Allocator>::data() noexcept {
		return m_data;
	}

	template<typename T, typename Allocator>
	inline typename storage_buffer<T, Allocator>::const_pointer storage_buffer<T, Allocator>::data() const noexcept {
		return m_data;
	}

}    // namespace CR::Core
