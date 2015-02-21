#pragma once
#include "Concepts.h"

template<Regular T>
struct DefaultOperatorsRegular
{
	bool operator!=(const T& a_other) const
	{
		return !(*static_cast<const T*>(this) == a_other);
	}
};

template<TotallyOrdered T>
struct DefaultOperatorsTotallyOrdered : public DefaultOperatorsRegular<T>
{
	bool operator<=(const T& a_other) const
	{
		return  !(a_other < *static_cast<const T*>(this));
	}

	bool operator>(const T& a_other) const
	{
		return (a_other < *static_cast<const T*>(this));
	}

	bool operator>=(const T& a_other) const
	{
		return !(*static_cast<const T*>(this) < a_other);
	}
};