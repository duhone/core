#include "catch.hpp"
#include "core/algorithm.h"
#include <vector>

using namespace std;
using namespace CR::Core;

TEST_CASE("for_each_argument", "") {
	vector<int> squares;
	auto square = [&](auto value) { squares.emplace_back(value * value); };

	for_each_argument(square, 0, 1, 2, 5, 10);

	REQUIRE(squares[0] == 0);
	REQUIRE(squares[1] == 1);
	REQUIRE(squares[2] == 4);
	REQUIRE(squares[3] == 25);
	REQUIRE(squares[4] == 100);
}
