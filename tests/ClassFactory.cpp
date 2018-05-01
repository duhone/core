#include "catch.hpp"
#include "ClassFactoryTestInterface.h"

TEST_CASE("class_factory", "basic tests")
{
  auto class1 = CreateInstance(TestClasses::Class1, 356);
	REQUIRE(class1.get());
	REQUIRE(class1->GetType() == TestClasses::Class1);
	REQUIRE(class1->GetValue() == 356);

	auto class2 = CreateInstance(TestClasses::Class2, 129);
	REQUIRE(class2.get());
	REQUIRE(class2->GetType() == TestClasses::Class2);
	REQUIRE(class2->GetValue() == 129);
}
