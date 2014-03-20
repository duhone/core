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
	}
}