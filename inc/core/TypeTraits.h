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

	// converts std:function style definition to a function pointer type. i.e. from
	// something like int(float) to int(*)(float)
	template<typename T>
	struct GetFunctionPtrType;

	template<typename RET, typename... Args>
	struct GetFunctionPtrType<RET(Args...)> {
		using type = RET (*)(Args...);
	};

	template<typename T>
	using GetFunctionPtrType_t = typename GetFunctionPtrType<T>::type;
}    // namespace CR::Core
