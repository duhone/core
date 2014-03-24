#include "Singleton.h"
#include "gtest/gtest.h"

using namespace CR::Core;

//namespace
//{
class OnlyOne : public Singleton<OnlyOne>
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

		int m_count{ 0 };
	};
//}

TEST(SingletonTest, Basics) {
	EXPECT_EQ(0, OnlyOne::Instance().GetCount());
	OnlyOne::Instance().IncCount();
	EXPECT_EQ(1, OnlyOne::Instance().GetCount());
	OnlyOne::Instance().IncCount();
	EXPECT_EQ(2, OnlyOne::Instance().GetCount());
}
