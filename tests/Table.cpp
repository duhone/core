#include <core/Table.h>

#include <3rdParty/doctest.h>

#include <type_traits>

namespace {
	struct Column {
		inline static int numConstructed{0};
		inline static int numDestructed{0};
		Column() { numConstructed++; }
		~Column() { numDestructed++; }

		uint32_t foo1{0};
		uint32_t foo2{1};
	};
	static_assert(std::is_standard_layout_v<Column>);
}    // namespace

TEST_CASE("table_simple") {
	using t_Table = CR::Core::Table<uint32_t, Column>;
	t_Table table{"simple table"};

	REQUIRE(table.GetIndex(0) == t_Table::c_unused);

	int preConstructed = Column::numConstructed;
	int preDestructed  = Column::numDestructed;

	[[maybe_unused]] uint16_t index = table.insert(0);
	REQUIRE(table.GetIndex(0) != t_Table::c_unused);
	REQUIRE(preConstructed == Column::numConstructed);
	REQUIRE(preDestructed == Column::numDestructed);

	auto& row = table.GetColumn<Column>(index);
	row.foo1  = 2;
	row.foo2  = 100;

	table.erase(index);
}
