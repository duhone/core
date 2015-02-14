#pragma once
#include <utility>
/*
template<typename Func, typename Tup, std::size_t... index>
decltype(auto) invoke_helper(Func&& func, Tup&& tup, std::index_sequence<index...>)
{
	return func(std::get<index>(std::forward<Tup>(tup))...);
}

template<typename Func, typename Tup>
decltype(auto) invoke(Func&& func, Tup&& tup)
{
	constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
	return invoke_helper(std::forward<Func>(func),
		std::forward<Tup>(tup),
		std::make_index_sequence<Size>{});
}
*/

namespace std17
{
	/*this is from standard proposal N3915. But it doesn't compile.
	namespace detail
	{
		template <class F, class Tuple, std::size_t... I>
		constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
		{
			return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
		}
	} // namespace detail

	template <class F, class Tuple>
	constexpr decltype(auto) apply(F&& f, Tuple&& t)
	{
		return apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
			std::make_index_sequence < std::tuple_size<std::decay_t<Tuple>>{} > {});
	}
	*/

	template<typename Func, typename Tup, std::size_t... index>
	decltype(auto) apply_impl(Func&& func, Tup&& tup, std::index_sequence<index...>)
	{
		return func(std::get<index>(std::forward<Tup>(tup))...);
	}

	template<typename Func, typename Tup>
	decltype(auto) apply(Func&& func, Tup&& tup)
	{
		constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
		return apply_impl(std::forward<Func>(func),
			std::forward<Tup>(tup),
			std::make_index_sequence<Size>{});
	}
}