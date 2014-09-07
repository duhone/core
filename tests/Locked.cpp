#include "gtest/gtest.h"
#include "Locked.h"
#include <future>
#include <algorithm>
#include <numeric>
#include "algorithm.h"

using namespace std;
using namespace CR::Core;

TEST(Locked, Basics) {
	Locked<vector<int>> data;
	
	auto task1 = async(std::launch::async, [&](){
		for (int i = 0; i < 10000; ++i)
		{
			data([i](auto& a_data) {
				a_data.push_back(i);
			});
		}
	});
	auto task2 = async(std::launch::async, [&](){
		for (int i = 0; i < 10000; ++i)
		{
			data([i](auto& a_data) {
				a_data.push_back(i);
			});
		}
	});
	//should crash or undefined behavior if Locked doesn't work
	task1.wait();
	task2.wait();

	const auto& cdata = data;

	auto task3 = async(std::launch::async, [&](){
		return cdata([](const auto& a_data) {
			return accumulate(a_data, 0);
		});
	});
	int result1 = task3.get();

	EXPECT_EQ(99990000, result1);
}
