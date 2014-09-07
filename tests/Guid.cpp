#include "gtest/gtest.h"
#include <unordered_set>
#include "Guid.h"

using namespace CR::Core;

TEST(Guid, Basics) {
	auto nullGuid = Guid::Null();	
	ASSERT_EQ(nullGuid, Guid( 0, 0, 0, 0 ));

	auto charGuid = Guid("{70C74876-2ABC-4B9A-A2D4-A8794F055A35}");
	ASSERT_EQ(charGuid.Data1(), 0x70c74876);
	ASSERT_EQ(charGuid.Data2(), 0x2abc4b9a);
	ASSERT_EQ(charGuid.Data3(), 0xa2d4a879);
	ASSERT_EQ(charGuid.Data4(), 0x4f055a35);
	ASSERT_EQ(charGuid, Guid(0x70c74876, 0x2abc4b9a, 0xa2d4a879, 0x4f055a35));
	ASSERT_NE(charGuid, Guid(0x70c74875, 0x2abc4b9a, 0xa2d4a879, 0x4f055a35));

	auto wcharGuid = Guid(L"{70C74876-2ABC-4B9A-A2D4-A8794F055A35}");
	ASSERT_EQ(wcharGuid, charGuid);

	auto stringGuid = Guid(std::string("{70C74876-2ABC-4B9A-A2D4-A8794F055A35}"));
	ASSERT_EQ(stringGuid, charGuid);

	std::set<Guid> guidsSet = {nullGuid, charGuid};
	auto findSet = guidsSet.find(charGuid);
	ASSERT_NE(findSet, std::end(guidsSet));

	std::unordered_set<Guid> guidsUnorderedSet = { nullGuid, charGuid };
	auto findUSet = guidsUnorderedSet.find(charGuid);
	ASSERT_NE(findUSet, std::end(guidsUnorderedSet));

	std::stringstream stream;
	stream << charGuid;
	EXPECT_EQ(stream.str(), std::string("{70C74876-2ABC-4B9A-A2D4-A8794F055A35}"));

	std::wstringstream wstream;
	wstream << charGuid;
	EXPECT_EQ(wstream.str(), std::wstring(L"{70C74876-2ABC-4B9A-A2D4-A8794F055A35}"));
}
