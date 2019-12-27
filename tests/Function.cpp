#include "catch.hpp"

#include "core/Function.h"

TEST_CASE("function", "basic tests") {
	CR::Core::MultiFunction<void(int, int)> multiFunc;
	int test1 = 0;
	int test2 = 0;
	multiFunc += [&](int arg1, int arg2) { test1 = arg1 + arg2; };
	multiFunc += [&](int arg1, int arg2) { test2 = arg1 * arg2; };
	REQUIRE(static_cast<bool>(multiFunc) == true);
	multiFunc(2, 3);
	REQUIRE(test1 == 5);
	REQUIRE(test2 == 6);

	test1 = 0;
	test2 = 0;
	CR::Core::SelectableFunction<int(int, int)> selFunc;
	selFunc += [&](int arg1, int arg2) { return arg1 + arg2; };
	selFunc += [&](int arg1, int arg2) { return arg1 * arg2; };
	selFunc.SetOperation(0);
	REQUIRE(static_cast<bool>(selFunc) == true);
	test1 = selFunc(5, 6);
	REQUIRE(test1 == 11);

	selFunc.SetOperation(1);
	REQUIRE(static_cast<bool>(selFunc) == true);
	test1 = selFunc(5, 6);
	REQUIRE(test1 == 30);
}
