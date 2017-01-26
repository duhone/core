#pragma once
#include <type_traits>

namespace CR::Core {
	template<typename T, typename... Rest>
	struct is_one_of;

	template<typename T>
	struct is_one_of<T> : std::false_type {};

	template<typename T, typename... Rest>
	struct is_one_of<T, T, Rest...> : std::true_type {};

	template<typename T, typename First, typename... Rest>
	struct is_one_of<T, First, Rest...> : is_one_of<T, Rest...> {};

	namespace Impl {
		template<typename, typename = void>
		struct HasTypeMember : public std::false_type {};

		template<typename T>
		struct HasTypeMember<T, std::void_t<typename T::type>> : public std::true_type {};
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

		//converts std:function style definition to a function pointer type. i.e. from 
		//something like int(float) to int(*)(float)
	template<typename T>
	struct GetFunctionPtrType;

	template<typename RET, typename... Args>
	struct GetFunctionPtrType<RET(Args...)> {
		using type = RET(*)(Args...);
	};

	template<typename T>
	using GetFunctionPtrType_t = typename GetFunctionPtrType<T>::type;
}
