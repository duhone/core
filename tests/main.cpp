#include "gtest/gtest.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();

#ifdef _MSC_VER
	if (IsDebuggerPresent())
	{
		cout << "Press return key to continue\n";
		cin.get();
	}
#endif

	return result;
}