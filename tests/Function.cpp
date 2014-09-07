#include "gtest\gtest.h"
#include "Function.h"

TEST(Function, Basics) {
	CR::Core::MultiFunction<void(int, int)> multiFunc;
	int test1 = 0;
	int test2 = 0;
	multiFunc += [&](int arg1, int arg2) {
		test1 = arg1 + arg2;
	};
	multiFunc += [&](int arg1, int arg2) {
		test2 = arg1*arg2;
	};
	EXPECT_EQ(true, static_cast<bool>(multiFunc));
	multiFunc(2, 3);
	EXPECT_EQ(5, test1);
	EXPECT_EQ(6, test2);

	test1 = 0;
	test2 = 0;
	CR::Core::SelectableFunction<int(int, int)> selFunc;
	selFunc += [&](int arg1, int arg2) {
		return arg1 + arg2;
	};
	selFunc += [&](int arg1, int arg2) {
		return arg1*arg2;
	};
	selFunc.SetOperation(0);
	EXPECT_EQ(true, static_cast<bool>(selFunc));
	test1 = selFunc(5, 6);
	EXPECT_EQ(11, test1);

	selFunc.SetOperation(1);
	EXPECT_EQ(true, static_cast<bool>(selFunc));
	test1 = selFunc(5, 6);
	EXPECT_EQ(30, test1);
}
