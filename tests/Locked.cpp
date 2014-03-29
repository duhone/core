#include "Locked.h"
#include "gtest/gtest.h"
#include <future>
#include <algorithm>

using namespace std;
using namespace CR::Core;

TEST(Locked, Basics) {
	Locked<vector<int>> data;
	
	auto task1 = async(std::launch::async, [&](){
		for (int i = 0; i < 10000; ++i)
		{
			data([i](vector<int>& data) {
				data.push_back(i);
			});
		}
	});
	auto task2 = async(std::launch::async, [&](){
		for (int i = 0; i < 10000; ++i)
		{
			data([i](vector<int>& data) {
				data.push_back(i);
			});
		}
	});
	//should crash or undefined behavior if Locked doesn't work
	task1.wait();
	task2.wait();

	const auto& cdata = data;

	auto task3 = async(std::launch::async, [&](){
		return cdata([](const vector<int>& data) {
			int sum1 = 0;
			for (const auto& i : data)
			{
				sum1 += i;
			}
			return sum1;
		});
	});
	int result1 = task3.get();

	EXPECT_EQ(99990000, result1);
}
