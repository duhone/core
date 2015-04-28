#pragma once
#include <type_traits>

namespace CR
{
	namespace Core
	{
		template<typename...>
		using void_t = void;

		template<typename T, typename... Rest>
		struct is_one_of;

		template<typename T>
		struct is_one_of<T> : std::false_type {};

		template<typename T, typename... Rest>
		struct is_one_of<T, T, Rest...> : std::true_type {};

		template<typename T, typename First, typename... Rest>
		struct is_one_of<T, First, Rest...> : is_one_of<T, Rest...> {};
		
		namespace Impl
		{
			template<typename, typename = void>
			struct HasMember : public std::false_type {};

			template<typename T>
			struct HasMember<T, void_t<typename T::type>> : public std::true_type{};
		}

		/*
		template<typename T>
		using copy_assignment_t = decltype(declval<T&>() = declval<T const&>());

		template<typename T, typename = vaoid>
		struct is_copy_assignable : public std::false_type {};

		template<typename T>
		struct is_copy_assignable<T, void_t<copy_assignment_t<T>>> : 
			public std::is_same<copy_assignment_t<T>, T&>{};
			*/
	}
}


/*
template<typename...>
struct voider { using type = void; }

template<typename... T0toN>
using void_t - typename voider<T0toN...>::type;
*/
