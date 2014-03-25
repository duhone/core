/* 
	versions of <algorithm> that operate on an entire container. for convienence
	and a few utilities
*/
#pragma once
#include <map>
#include <algorithm>
#include <vector>

namespace CR
{
	namespace Core
	{
		template<class T,class F>
		void ForEach(T& list,F& ftor)
		{
			std::for_each(std::begin(list),std::end(list),ftor);
		};

		template<class T,class S,class F>
		void Transform(T& src,S& dst,F& ftor)
		{
			std::transform(std::begin(src),std::end(src),std::begin(dst),ftor);
		};

		template<class T,class V>
		void Fill(T& list,V _value)
		{
			std::fill(std::begin(list.begin()),std::end(list.end()),_value);
		};

		template<class T, class F>
		bool AllOf(T& list, F& ftor)
		{
			return std::all_of(std::begin(list), std::end(list), ftor);
		}

		template<class T, class F>
		bool AnyOf(T& list, F& ftor)
		{
			return std::any_of(std::begin(list), std::end(list), ftor);
		}

		template<class T,class S>
		void UnorderedRemove(T& _container,S& _value)
		{
			_value = _container.back();
			_container.pop_back();
		}

		template<typename Container>
		void EraseRemove(Container& a_container, const typename Container::value_type& a_value)
		{
			a_container.erase(std::remove(std::begin(a_container), std::end(a_container), a_value), std::end(a_container));
		}

		template<typename Container, typename Predicate>
		void EraseRemoveIf(Container& a_container, Predicate a_pred)
		{
			a_container.erase(std::remove_if(begin(a_container), end(a_container), a_pred), end(a_container));
		}

		template<typename Container, typename CompareT>
		std::size_t SortedInsert(Container& a_container, const typename Container::value_type& a_item, CompareT a_compare)
		{
			auto it = std::begin(a_container);
			for (; it != std::end(a_container); ++it)
			{
				if (a_compare(*it, a_item))
					break;
			}
			it = a_container.insert(it, a_item);
			return std::distance(a_container.begin(), it);
		}

		template<typename T>
		std::size_t Size(const T& x)
		{ 
			return x.size();
		}

		template<typename T, std::size_t N>
		std::size_t Size(const T(&x)[N])
		{ 
			return N;
		}

	}
}