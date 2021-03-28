#pragma once

#include <core/Log.h>
#include <core/TypeTraits.h>

#include <3rdParty/robinmap.h>

#include <array>
#include <bitset>
#include <cinttypes>
#include <tuple>
#include <type_traits>
#include <utility>

namespace CR::Core {
	// t_column... is a struct(or primitive) holding the data for one column of a row. t_column... should not contain
	// the primary key. You can have more than one t_column type. This allows creating a SOAOS layout. The primary key
	// is in one array, and each column in an array. You can iterate the rows of the table for just one column. You can
	// grab multiple iterators if you need to walk multiple columns at once. The fastest way to get to a row for a
	// column is by index, not by primary key, you can save these indices for future use. In general indices are stable.
	// The function compact will invalidate them however, there is rarely a need to call compact.
	template<typename t_primaryKey, typename... t_columns>
	class Table {
		static_assert(is_unique_v<t_columns...>, "column types must be unique currently");

		// Using arrays currently, and std::bitset. For larger tables need a bitset replacement(which would help here
		// too). And should use std::vector instead of arrays for larger tables.
		static constexpr uint16_t c_maxSize = 512;
		// If column is larger than this, should refactor. Bulk data should be stored elsewhere on the heap(i.e.
		// unique_ptr, or a vector). If still larger than this, then you probably aren't in boyce-codd normal form. Or
		// you just need to split up the table. You can of course have multiple columns.
		static constexpr uint32_t c_maxRowSize = 64;
		static_assert((... && (sizeof(t_columns) <= c_maxRowSize)));

	  public:
		// A system wide constant, need to find a better place for it. Its duplicated in many public interfaces
		inline static constexpr uint16_t c_unused{0xffff};

		Table(std::string_view a_tableName) : m_tableName(a_tableName){};
		~Table() = default;

		Table(const Table&) = delete;
		Table(Table&&)      = delete;

		Table& operator=(const Table&) = delete;
		Table& operator=(Table&&) = delete;

		// returns the new index, fatal error if table is full
		uint16_t insert(t_primaryKey&& a_key, t_columns&&... a_row);
		// row will be default constructed if not standard layout, otherwise uninitialized
		uint16_t insert(t_primaryKey&& a_key);

		// problematic when t_primaryKey is a string type(char*, string, string_view, ect) should just take a
		// string_view for those. Wait until C++20 to fix, can be done cleaner there.
		[[nodiscard]] uint16_t GetIndex(const t_primaryKey& a_key) const;

		template<typename t_column>
		[[nodiscard]] const t_column& GetColumn(uint16_t a_index) const;
		template<typename t_column>
		[[nodiscard]] t_column& GetColumn(uint16_t a_index);

		void erase(uint16_t a_index);

	  private:
		// returns c_maxSize if couldn't find one
		[[nodiscard]] uint16_t FindUnused();

		template<size_t index, typename t_column>
		void ClearColumn(uint16_t a_index, t_column&& a_column);
		template<size_t... tupleIndices>
		void ClearRow(uint16_t a_index, std::index_sequence<tupleIndices...>, t_columns&&... row);

		std::string m_tableName;
		std::bitset<c_maxSize> m_used;
		t_primaryKey m_primaryKeys[c_maxSize];
		tsl::robin_map<t_primaryKey, uint16_t> m_lookUp;
		std::tuple<std::array<t_columns, c_maxSize>...> m_rows;
	};
}    // namespace CR::Core

template<typename t_primaryKey, typename... t_columns>
inline uint16_t CR::Core::Table<t_primaryKey, t_columns...>::FindUnused() {
	// TODO: std::bitset sucks, unless its 64 bits or less, can't do a bitscan manually even.
	for(uint16_t i = 0; i < c_maxSize; ++i) {
		if(!m_used[i]) { return i; }
	}
	return c_maxSize;
}

template<typename t_primaryKey, typename... t_columns>
template<size_t index, typename t_column>
void CR::Core::Table<t_primaryKey, t_columns...>::ClearColumn([[maybe_unused]] uint16_t a_index,
                                                              [[maybe_unused]] t_column&& a_column) {
	if constexpr(!std::is_standard_layout_v<t_column>) { std::get<index>(m_rows)[a_index] = std::move(a_column); }
}

template<typename t_primaryKey, typename... t_columns>
template<size_t... tupleIndices>
void CR::Core::Table<t_primaryKey, t_columns...>::ClearRow(uint16_t a_index, std::index_sequence<tupleIndices...>,
                                                           t_columns&&... a_row) {
	(ClearColumn<tupleIndices>(a_index, std::move(a_row)), ...);
}

template<typename t_primaryKey, typename... t_columns>
inline uint16_t CR::Core::Table<t_primaryKey, t_columns...>::insert(t_primaryKey&& a_key, t_columns&&... a_row) {
	uint16_t unusedIndex = insert(a_key);

	ClearRow(unusedIndex, std::index_sequence_for<t_rows...>{}, std::move(a_row)...);

	return unusedIndex;
}

template<typename t_primaryKey, typename... t_columns>
inline uint16_t CR::Core::Table<t_primaryKey, t_columns...>::insert(t_primaryKey&& a_key) {
	Log::Require(m_lookUp.find(a_key) == std::end(m_lookUp), "Tried to insert, but row already exists with this key {}",
	             m_tableName);

	uint16_t unusedIndex = FindUnused();
	Log::Require(unusedIndex != c_maxSize, "Ran out of available rows in table {}", m_tableName);

	m_lookUp.emplace(a_key, unusedIndex);
	m_primaryKeys[unusedIndex] = std::move(a_key);

	m_used[unusedIndex] = true;

	return unusedIndex;
}

template<typename t_primaryKey, typename... t_columns>
inline uint16_t CR::Core::Table<t_primaryKey, t_columns...>::GetIndex(const t_primaryKey& a_key) const {
	auto iter = m_lookUp.find(a_key);
	if(iter == m_lookUp.end()) {
		return c_unused;
	} else {
		return iter->second;
	}
}

template<typename t_primaryKey, typename... t_columns>
template<typename t_row>
inline const t_row& CR::Core::Table<t_primaryKey, t_columns...>::GetColumn(uint16_t a_index) const {
	Log::Assert(a_index != c_unused && m_used[a_index], "asked for an unused row in table {}", m_tableName);

	return std::get<std::array<t_row, c_maxSize>>(m_rows)[a_index];
}

template<typename t_primaryKey, typename... t_columns>
template<typename t_row>
inline t_row& CR::Core::Table<t_primaryKey, t_columns...>::GetColumn(uint16_t a_index) {
	Log::Assert(a_index != c_unused && m_used[a_index], "asked for an unused row in table {}", m_tableName);

	return std::get<std::array<t_row, c_maxSize>>(m_rows)[a_index];
}

template<typename t_primaryKey, typename... t_columns>
inline void CR::Core::Table<t_primaryKey, t_columns...>::erase(uint16_t a_index) {
	Log::Assert(a_index != c_unused && m_used[a_index], "tried to delete an unused row in table {}", m_tableName);

	m_used[a_index] = false;

	// To save on memory, if not a standard layout, then clear out the data. Hopefully if not standard layout
	// then a proper move assignment was written.
	if constexpr(!std::is_standard_layout_v<t_primaryKey>) { m_primaryKeys[a_index] = t_primaryKey{}; }
	ClearRow(a_index, std::index_sequence_for<t_columns...>{}, t_columns{}...);
}
