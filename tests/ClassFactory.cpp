#include "ClassFactoryTestInterface.h"
#include "gtest\gtest.h"

TEST(ClassFactory, Basics) {
	auto class1 = TestFactory::Instance().Create(TestClasses::Class1, 356);
	EXPECT_NE(nullptr, class1.get());
	EXPECT_EQ(TestClasses::Class1, class1->GetType());
	EXPECT_EQ(356, class1->GetValue());

	auto class2 = TestFactory::Instance().Create(TestClasses::Class2, 129);
	EXPECT_NE(nullptr, class2.get());
	EXPECT_EQ(TestClasses::Class2, class2->GetType());
	EXPECT_EQ(129, class2->GetValue());
}
