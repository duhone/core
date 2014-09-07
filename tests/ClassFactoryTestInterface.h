#pragma once
#include "ClassFactory.h"

enum class TestClasses
{
	Class1,
	Class2
};

class TestInterface
{
public:
	virtual TestClasses GetType() const = 0;
	virtual int GetValue() const = 0;
};

using TestFactory = CR::Core::ClassFactory<std::unique_ptr<TestInterface>, TestClasses, int>;
