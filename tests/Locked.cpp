#include "catch.hpp"
#include "Locked.h"
#include <future>
#include <algorithm>
#include <numeric>
#include <vector>
#include <list>
#include "algorithm.h"

using namespace std;
using namespace CR::Core;

TEST_CASE("Locked", "basic tests")
{
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

	REQUIRE(result1 == 99990000);
}

TEST_CASE("Locked Multiple", "multiple types")
{
	Locked<vector<int>, list<float>> data;

	auto task1 = async(std::launch::async, [&]() {
		for(int i = 0; i < 5; ++i)
		{
			data([i](auto& a_intVec, auto& a_floatList) {
				a_intVec.push_back(i);
				a_floatList.push_back(2.0f * i);
			});
		}
	});
	auto task2 = async(std::launch::async, [&]() {
		for(int i = 5; i < 10; ++i)
		{
			data([i](auto& a_intVec, auto& a_floatList) {
				a_intVec.push_back(i);
				a_floatList.push_back(2.0f * i);
			});
		}
	});
	task1.wait();
	task2.wait();

	const auto& cdata = data;

	auto task3 = async(std::launch::async, [&]() {
		return cdata([](const auto& a_intVec, const auto& a_floatList) {
			return make_tuple(accumulate(a_intVec, 0), accumulate(a_floatList, 0));
		});
	});
	auto result1 = task3.get();

	REQUIRE(get<0>(result1) == 45);
	REQUIRE(get<1>(result1) == 90);
}