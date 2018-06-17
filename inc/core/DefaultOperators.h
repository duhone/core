#pragma once
#include "core/Concepts.h"

namespace CR::Core {
	// Your class must implement ==
	template<Regular T>
	struct DefaultOperatorsRegular {
		friend bool operator!=(const T& a_arg1, const T& a_arg2) { return !(a_arg1 == a_arg2); }
	};

	// Your class must implement <
	template<StrictWeakOrdering T>
	struct DefaultOperatorsStrictWeakOrdering {
		friend bool operator>(const T& a_arg1, const T& a_arg2) { return (a_arg2 < a_arg1); }
	};

	// Your class must implement == and <
	template<TotallyOrdered T>
	struct DefaultOperatorsTotallyOrdered : public DefaultOperatorsRegular<T>,
	                                        public DefaultOperatorsStrictWeakOrdering<T> {
		friend bool operator<=(const T& a_arg1, const T& a_arg2) { return !(a_arg2 < a_arg1); }

		friend bool operator>=(const T& a_arg1, const T& a_arg2) { return !(a_arg1 < a_arg2); }
	};

	// Your class must implement ==, <, +=, -=, *=, /=
	// You may be able to do better than this base class. This class does not assume any operations are communitive
	// so it could be used by a matrix class for instance. This means you may be able to beat the performance of
	// the (const T&, T&&) variants of this function if your type is communitive. You can also probably do better for
	// classes like string where using the larger of the 2 args as the left arg could yeild improved performance. If
	// your class is very small, then the ineffeciancy of the (const T&, T&&) variant is probably irellevant.
	template<Arithmetic T>
	struct DefaultOperatorsArithmetic : public DefaultOperatorsTotallyOrdered<T> {
		friend T operator+(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result += a_arg2;
			return result;
		}
		friend T operator+(T&& a_arg1, const T& a_arg2) {
			a_arg1 += a_arg2;
			return std::move(a_arg1);
		}
		friend T operator+(const T& a_arg1, T&& a_arg2) {
			T result{a_arg1};
			result += a_arg2;
			return result;
		}
		friend T operator+(T&& a_arg1, T&& a_arg2) {
			a_arg1 += a_arg2;
			return std::move(a_arg1);
		}
		friend T operator-(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result -= a_arg2;
			return result;
		}
		friend T operator-(T&& a_arg1, const T& a_arg2) {
			a_arg1 -= a_arg2;
			return std::move(a_arg1);
		}
		friend T operator-(const T& a_arg1, T&& a_arg2) {
			T result{a_arg1};
			result -= a_arg2;
			return result;
		}
		friend T operator-(T&& a_arg1, T&& a_arg2) {
			a_arg1 -= a_arg2;
			return std::move(a_arg1);
		}
		friend T operator*(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result *= a_arg2;
			return result;
		}
		friend T operator*(T&& a_arg1, const T& a_arg2) {
			a_arg1 *= a_arg2;
			return std::move(a_arg1);
		}
		friend T operator*(const T& a_arg1, T&& a_arg2) {
			T result{a_arg1};
			result *= a_arg2;
			return result;
		}
		friend T operator*(T&& a_arg1, T&& a_arg2) {
			a_arg1 *= a_arg2;
			return std::move(a_arg1);
		}
		friend T operator/(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result /= a_arg2;
			return result;
		}
		friend T operator/(T&& a_arg1, const T& a_arg2) {
			a_arg1 /= a_arg2;
			return std::move(a_arg1);
		}
		friend T operator/(const T& a_arg1, T&& a_arg2) {
			T result{a_arg1};
			result /= a_arg2;
			return result;
		}
		friend T operator/(T&& a_arg1, T&& a_arg2) {
			a_arg1 /= a_arg2;
			return std::move(a_arg1);
		}
	};

	// Your class must implement ==, <, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
	// This class assumes T is a small object that would not benifit from a move.
	// The operations here are really only appropriate semanticly for integers and maybe
	// a collection of integers like int4.
	template<IntegerArithmetic T>
	struct DefaultOperatorsIntegerArithmetic : public DefaultOperatorsArithmetic<T> {
		friend T operator%(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result %= a_arg2;
			return result;
		}
		friend T operator&(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result &= a_arg2;
			return result;
		}
		friend T operator|(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result |= a_arg2;
			return result;
		}
		friend T operator^(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result ^= a_arg2;
			return result;
		}
		friend T operator<<(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result <<= a_arg2;
			return result;
		}
		friend T operator>>(const T& a_arg1, const T& a_arg2) {
			T result{a_arg1};
			result >>= a_arg2;
			return result;
		}
	};
}    // namespace CR::Core