#pragma once
#include <functional>

enum class TestClasses { Class1, Class2 };

class TestInterface {
  public:
	virtual ~TestInterface() = default;

	virtual TestClasses GetType() const = 0;
	virtual int GetValue() const        = 0;
};

bool RegisterCreator(TestClasses classType, std::function<std::unique_ptr<TestInterface>(int)> creater);
std::unique_ptr<TestInterface> CreateInstance(TestClasses classType, int value);
