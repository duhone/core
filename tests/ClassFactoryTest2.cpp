#include "ClassFactoryTestInterface.h"

namespace {
	class Class2 final : public TestInterface {
	  public:
		Class2(int a_arg) : m_value(a_arg) {}

	  private:
		TestClasses GetType() const override { return TestClasses::Class2; }
		int GetValue() const override { return m_value; }

		int m_value{0};
	};

	const bool registered = RegisterCreator(TestClasses::Class2, [](int arg) { return std::make_unique<Class2>(arg); });
}    // namespace
