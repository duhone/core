#include "catch.hpp"
#include "Singleton.h"

using namespace CR::Core;

namespace
{
class OnlyOne final : public Singleton<OnlyOne>
	{
	public:
		friend Singleton<OnlyOne>;

		void IncCount() { ++m_count; }
		int GetCount() const { return m_count; }
	private:
		OnlyOne() = default;
		~OnlyOne() = default;
		OnlyOne(const OnlyOne&) = delete;
		OnlyOne(OnlyOne&&) = delete;
		OnlyOne& operator=(const OnlyOne&) = delete;
		OnlyOne& operator=(OnlyOne&&) = delete;

		int m_count{ 0 };
	};
}

TEST_CASE("singleton", "basic tests")
{
	REQUIRE(OnlyOne::Instance().GetCount() == 0);
	OnlyOne::Instance().IncCount();
	REQUIRE(OnlyOne::Instance().GetCount() == 1);
	OnlyOne::Instance().IncCount();
	REQUIRE(OnlyOne::Instance().GetCount() == 2);
}
