#include "ClassFactoryTestInterface.h"
#include "core/ClassFactory.h"

using namespace CR::Core;
using namespace std;

auto& GetInstance() {
	static ClassFactory<unique_ptr<TestInterface>, TestClasses, int> instance;
	return instance;
}

bool RegisterCreator(TestClasses classType, function<unique_ptr<TestInterface>(int)> creater) {
	GetInstance().RegisterCreator(classType, creater);
	return true;
}

std::unique_ptr<TestInterface> CreateInstance(TestClasses classType, int value) {
	return GetInstance().Create(classType, value);
}
