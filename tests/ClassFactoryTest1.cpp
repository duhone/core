#include "ClassFactoryTestInterface.h"

namespace
{
	class Class1 final : public TestInterface
	{
	public:
		Class1(int a_arg) : m_value(a_arg) {}

	private:
		TestClasses GetType() const override { return TestClasses::Class1; }
		int GetValue() const override { return m_value; }

		int m_value{ 0 };
	};

	const bool registered = TestFactory::Instance().RegisterCreator(TestClasses::Class1, [](int arg){return std::make_unique<Class1>(arg); });
}
