#include <core/Table.h>

#include <3rdParty/doctest.h>

#include <type_traits>

namespace {
	struct Row {
		inline static int numConstructed{0};
		inline static int numDestructed{0};
		Row() { numConstructed++; }
		~Row() { numDestructed++; }

		uint32_t foo1{0};
		uint32_t foo2{1};
	};
	static_assert(std::is_standard_layout_v<Row>);
}    // namespace

TEST_CASE("table_simple") {
	using t_Table = CR::Core::Table<uint32_t, Row>;
	t_Table table{"simple table"};

	REQUIRE(table.GetIndex(0) == t_Table::c_unused);

	// int preConstructed = Row::numConstructed;
	// int preDestructed  = Row::numDestructed;

	[[maybe_unused]] uint16_t index = table.CreateRow(0);
	REQUIRE(table.GetIndex(0) != t_Table::c_unused);
	// REQUIRE(preConstructed == Row::numConstructed);
	// REQUIRE(preDestructed == Row::numDestructed);

	auto& row = table.GetRow<Row>(index);
	row.foo1  = 2;
	row.foo2  = 100;

	table.DeleteRow(index);
}
