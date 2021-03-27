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

	template<typename T, typename... Rest>
	using is_one_of_t = typename is_one_of<T, Rest...>::type;

	template<typename T, typename... Rest>
	inline constexpr bool is_one_of_v = is_one_of<T, Rest...>::value;

	template<typename T, typename... Rest>
	struct is_unique;

	template<typename T>
	struct is_unique<T> : std::true_type {};

	template<typename T, typename... Rest>
	struct is_unique<T, T, Rest...> : std::false_type {};

	template<typename T, typename First, typename... Rest>
	struct is_unique<T, First, Rest...> : is_unique<T, Rest...> {};

	template<typename T, typename... Rest>
	using is_unique_t = typename is_unique<T, Rest...>::type;

	template<typename T, typename... Rest>
	inline constexpr bool is_unique_v = is_unique<T, Rest...>::value;

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
