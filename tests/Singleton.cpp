#include "Singleton.h"
#include "gtest/gtest.h"

using namespace CR::Core;

//namespace
//{
	class OnlyOne
	{
	public:
		friend Singleton<OnlyOne>;
		friend std::unique_ptr<OnlyOne>;

		void IncCount() { ++m_count; }
		int GetCount() const { return m_count; }
	private:
		OnlyOne() {}
		~OnlyOne() {}
		OnlyOne(const OnlyOne&) = delete;
		OnlyOne(OnlyOne&&) = delete;
		OnlyOne& operator=(const OnlyOne&) = delete;

		int m_count{ 0 };
	};
//}

TEST(SingletonTest, Basics) {
	typedef Singleton<OnlyOne> MySingleton;

	EXPECT_EQ(0, MySingleton::Instance().GetCount());
	MySingleton::Instance().IncCount();
	EXPECT_EQ(1, MySingleton::Instance().GetCount());
	MySingleton::Instance().IncCount();
	EXPECT_EQ(2, MySingleton::Instance().GetCount());
}
