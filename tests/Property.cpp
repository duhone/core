#include "catch.hpp"
#include "core/Property.h"

class PropertyTest
{
public:
	CR::Core::Property<int, PropertyTest> Data{this, &PropertyTest::GetData, &PropertyTest::SetData};
private:
	void SetData(const int& data) { m_data = data; }
	int& GetData() { return m_data; }
	int m_data{0};
};

struct Foo
{
	bool operator==(const Foo& a_arg) { return Value == a_arg.Value; }
	bool operator!=(const Foo& a_arg) { return !((*this) == a_arg); }
	int Value{0};
};

class PropertyTest2
{
public:
	//CR::Core::Property<int, PropertyTest2> Data{this, &PropertyTest2::GetData, &PropertyTest2::SetData};
	CRProperty(Foo, PropertyTest2, GetData, SetData);
private:
	void SetData(const Foo& data) { m_data = data; }
	Foo& GetData() { return m_data; }
	Foo m_data;
};

TEST_CASE("Property", "basic tests")
{
	PropertyTest test;
	REQUIRE(test.Data == 0);
	test.Data = 2;
	REQUIRE(test.Data == 2);
	PropertyTest test2;
	test2.Data = 2;
	REQUIRE(test.Data == test2.Data);
	test2.Data = 3;
	REQUIRE(test.Data != test2.Data);
	REQUIRE(2 != test2.Data);
	REQUIRE(test.Data < test2.Data);
	REQUIRE(test2.Data > test.Data);
	//test2.Data += 2;
	//REQUIRE(test2.Data == 5);

	//Still not sure how to make this work
	//PropertyTest2 test3;
	//REQUIRE(test3.Data.Value == 0);

}
