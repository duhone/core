/*
	versions of <algorithm> that operate on an entire container. for convienence
	and a few utilities
	*/
#pragma once
#include <map>
#include <algorithm>
#include <numeric>
#include <vector>
#include <initializer_list>
#include "core/Concepts.h"

namespace CR::Core {
	template<Container ContainerT, Callable CallableT>
	void for_each(ContainerT& a_container, CallableT a_callable) {
		std::for_each(std::begin(a_container), std::end(a_container), a_callable);
	};

	template<Container ContainerSrcT, Container ContainerDstT>
	void copy(const ContainerSrcT& a_src, ContainerDstT& a_dst) {
		std::copy(std::cbegin(a_src), std::cend(a_src), std::begin(a_dst));
	};

	template<Container ContainerSrcT, Container ContainerDstT, Callable CallableT>
	void transform(const ContainerSrcT& a_src, ContainerDstT& a_dst, CallableT a_callable) {
		std::transform(std::cbegin(a_src), std::cend(a_src), std::begin(a_dst), a_callable);
	};

	template<Container ContainerT, SemiRegular V>
	void fill(ContainerT& a_container, V a_value) {
		std::fill(std::begin(a_container), std::end(a_container), a_value);
	};

	template<Container ContainerT1, Container ContainerT2>
	bool equal(const ContainerT1& a_container1, const ContainerT2& a_container2) {
		return std::equal(std::cbegin(a_container1), std::cend(a_container1),
						  std::cbegin(a_container2), std::cend(a_container2));
	}

	template<Container ContainerT1, Container ContainerT2, Predicate PredicateT>
	bool equal(const ContainerT1& a_container1, const ContainerT2& a_container2, PredicateT a_predicate) {
		return std::equal(std::cbegin(a_container1), std::cend(a_container1),
						  std::begin(a_container2), std::cend(a_container2), a_predicate);
	}

	template<Container ContainerT, Callable CallableT>
	bool all_of(const ContainerT& a_container, CallableT a_callable) {
		return std::all_of(std::cbegin(a_container), std::cend(a_container), a_callable);
	}

	template<Container ContainerT, Callable CallableT>
	bool any_of(const ContainerT& a_container, CallableT a_callable) {
		return std::any_of(std::cbegin(a_container), std::cend(a_container), a_callable);
	}

	template<Container ContainerT, InputIterator InputIteratorT>
	void UnorderedRemove(ContainerT& a_container, InputIteratorT& a_iterator) {
		*a_iterator = *a_container.back();
		a_container.pop_back();
	}

	template<Container ContainerT>
	void EraseRemove(ContainerT& a_container, const typename ContainerT::value_type& a_value) {
		a_container.erase(std::remove(std::begin(a_container), std::end(a_container), a_value),
						  std::end(a_container));
	}

	template<Container ContainerT, Predicate PredicateT>
	void EraseRemoveIf(ContainerT& a_container, PredicateT a_predicate) {
		a_container.erase(std::remove_if(begin(a_container), end(a_container), a_predicate),
						  end(a_container));
	}

	template<Container ContainerT, Comparator CompareT>
	std::size_t SortedInsert(ContainerT& a_container, const typename ContainerT::value_type& a_value,
							 CompareT a_compare) {
		auto it = std::upper_bound(begin(a_container), end(a_container), a_value, a_compare);
		it = a_container.insert(it, a_value);
		return std::distance(a_container.begin(), it);
	}

	template<Container ContainerT>
	typename ContainerT::value_type accumulate(const ContainerT& a_container,
											   typename ContainerT::value_type a_initialValue) {
		return std::accumulate(std::cbegin(a_container), std::cend(a_container), a_initialValue);
	}

	template<Container ContainerT, Callable CallableT>
	typename ContainerT::value_type accumulate(const ContainerT& a_container,
											   typename ContainerT::value_type a_initialValue, CallableT a_callable) {
		return std::accumulate(std::cbegin(a_container), std::cend(a_container), a_initialValue, a_callable);
	}

	template<Callable CallableT, typename... Ts>
	void for_each_argument(CallableT&& a_callable, Ts&&... a_args) {
		(void)std::initializer_list<int> {(a_callable(std::forward<Ts>(a_args)), 0)...};
	}
}
