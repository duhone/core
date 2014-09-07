#pragma once
#include<algorithm>

namespace CR
{
	namespace Core
	{
		//sinlge iteration of insertion sort. fast if you are rarely inserting new elements into
		//	an already sorted list.
		template<class T, class S, class F>
		void InsertIntoSorted(T &_container, S _element, F _predicate)
		{
			T::iterator iterator = std::lower_bound(_container.begin(), _container.end(), _element, _predicate);
			if (iterator == _container.end())
				_container.push_back(_element);
			else
				_container.insert(iterator, _element);
		}
	}
}
