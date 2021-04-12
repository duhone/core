#include <core/Table.h>

#include <3rdParty/doctest.h>

#include <type_traits>

namespace {
	struct PlayerHealth {
		inline static int numConstructed{0};
		inline static int numDestructed{0};
		PlayerHealth() { numConstructed++; }
		~PlayerHealth() { numDestructed++; }

		uint32_t Health{100};
		uint32_t Armor{100};
	};
	static_assert(std::is_standard_layout_v<PlayerHealth>);

	struct PlayerMoney {
		inline static int numConstructed{0};
		inline static int numDestructed{0};
		PlayerMoney() { numConstructed++; }
		~PlayerMoney() { numDestructed++; }

		uint32_t Copper{0};
		uint32_t Silver{0};
		uint32_t Gold{0};
		uint32_t Platinum{0};
	};
	static_assert(std::is_standard_layout_v<PlayerMoney>);
}    // namespace

TEST_CASE("table_simple") {
	using t_Table = CR::Core::Table<uint32_t, PlayerHealth>;
	t_Table table{"simple table"};

	REQUIRE(table.GetIndex(0) == t_Table::c_unused);

	int preConstructed = PlayerHealth::numConstructed;
	int preDestructed  = PlayerHealth::numDestructed;

	uint16_t index1 = table.insert(0);
	REQUIRE(table.GetIndex(0) != t_Table::c_unused);
	REQUIRE(preConstructed == PlayerHealth::numConstructed);
	REQUIRE(preDestructed == PlayerHealth::numDestructed);

	// first one damaged
	auto& row  = table.GetColumn<PlayerHealth>(index1);
	row.Health = 2;
	row.Armor  = 100;

	// add a second one
	[[maybe_unused]] uint16_t index2 = table.insert(1);
	REQUIRE(table.GetIndex(1) != t_Table::c_unused);

	// reduce armor for all
	auto columnSet = table.GetColumnSet<PlayerHealth>();

	for(auto& [health] : columnSet) { health.get().Armor = 50; }

	// still holding ref to first row, check it has expected values
	REQUIRE(row.Health == 2);
	REQUIRE(row.Armor == 50);

	// now check second row
	row = table.GetColumn<PlayerHealth>(index2);
	REQUIRE(row.Health == 100);
	REQUIRE(row.Armor == 50);

	table.erase(index1);
	table.erase(index2);

	// should have never created or destroyed any.
	REQUIRE(preConstructed == PlayerHealth::numConstructed);
	REQUIRE(preDestructed == PlayerHealth::numDestructed);
}
