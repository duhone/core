#pragma once

#include <core/Log.h>

#include <3rdParty/robinmap.h>

#include <array>
#include <bitset>
#include <cinttypes>
#include <tuple>
#include <type_traits>
#include <utility>

namespace CR::Core {
	// t_row... is a struct holding the data for  row. t_row... should not contain the primary key.
	// You can have more than one t_row struct. This allows creating a SOAOS layout. The primary key is in one array,
	// and each row in in an array. You can iterate just one row, or a multiple of rows at once.
	// The fastest way to get to a row is by index, not by primary key, you can save these indices for future use. In
	// general indices are stable. The function compact will invalidate them however, there is rarely a need to call
	// compact.
	template<typename t_primaryKey, typename... t_rows>
	class Table {
		// Using arrays currently, and std::bitset. For larger tables need a bitset replacement(which would help here
		// too). And should use std::vector instead of arrays for larger tables.
		static constexpr uint16_t c_maxSize = 512;
		// If row is larger than this, should refactor. Bulk data should be stored elsewhere on the heap(i.e.
		// unique_ptr, or a vector). If still larger than this, then you probably aren't in boyce-codd normal form. Or
		// you just need to split up the table.
		static constexpr uint32_t c_maxRowSize = 64;
		static_assert((... && (sizeof(t_rows) <= c_maxRowSize)));

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
		uint16_t CreateRow(t_primaryKey&& a_key, t_rows&&... a_row);
		// row will be default constructed if not standard layout, otherwise uninitialized
		uint16_t CreateRow(t_primaryKey&& a_key);

		// problematic when t_primaryKey is a string type(char*, string, string_view, ect) should just take a
		// string_view for those. Wait until C++20 to fix, can be done cleaner there.
		[[nodiscard]] uint16_t GetIndex(const t_primaryKey& a_key) const;

		template<typename t_row>
		[[nodiscard]] const t_row& GetRow(uint16_t a_index) const;
		template<typename t_row>
		[[nodiscard]] t_row& GetRow(uint16_t a_index);

		void DeleteRow(uint16_t a_index);

	  private:
		// returns c_maxSize if couldn't find one
		[[nodiscard]] uint16_t FindUnused();

		template<size_t index, typename t_row>
		void ClearRow(uint16_t a_index, t_row&& a_row);
		template<size_t... tupleIndices>
		void ClearRows(uint16_t a_index, std::index_sequence<tupleIndices...>, t_rows&&... rows);

		std::string m_tableName;
		std::bitset<c_maxSize> m_used;
		t_primaryKey m_primaryKeys[c_maxSize];
		tsl::robin_map<t_primaryKey, uint16_t> m_lookUp;
		std::tuple<std::array<t_rows, c_maxSize>...> m_rows;
	};
}    // namespace CR::Core

template<typename t_primaryKey, typename... t_rows>
inline uint16_t CR::Core::Table<t_primaryKey, t_rows...>::FindUnused() {
	// TODO: std::bitset sucks, unless its 64 bits or less, can't do a bitscan manually even.
	for(uint16_t i = 0; i < c_maxSize; ++i) {
		if(!m_used[i]) { return i; }
	}
	return c_maxSize;
}

template<typename t_primaryKey, typename... t_rows>
template<size_t index, typename t_row>
void CR::Core::Table<t_primaryKey, t_rows...>::ClearRow([[maybe_unused]] uint16_t a_index,
                                                        [[maybe_unused]] t_row&& a_row) {
	if constexpr(!std::is_standard_layout_v<t_row>) { std::get<index>(m_rows)[a_index] = std::move(a_row); }
}

template<typename t_primaryKey, typename... t_rows>
template<size_t... tupleIndices>
void CR::Core::Table<t_primaryKey, t_rows...>::ClearRows(uint16_t a_index, std::index_sequence<tupleIndices...>,
                                                         t_rows&&... a_rows) {
	(ClearRow<tupleIndices>(a_index, std::move(a_rows)), ...);
}

template<typename t_primaryKey, typename... t_rows>
inline uint16_t CR::Core::Table<t_primaryKey, t_rows...>::CreateRow(t_primaryKey&& a_key, t_rows&&... a_rows) {
	uint16_t unusedIndex = FindUnused();
	Log::Require(unusedIndex != c_maxSize, "Ran out of available rows in table {}", m_tableName);

	m_lookUp.emplace(a_key, unusedIndex);
	m_primaryKeys[unusedIndex] = std::move(a_key);
	ClearRows(unusedIndex, std::index_sequence_for<t_rows...>{}, std::move(a_rows)...);

	m_used[unusedIndex] = true;

	return unusedIndex;
}

template<typename t_primaryKey, typename... t_rows>
inline uint16_t CR::Core::Table<t_primaryKey, t_rows...>::CreateRow(t_primaryKey&& a_key) {
	return CreateRow(std::move(a_key), t_rows{}...);
}

template<typename t_primaryKey, typename... t_rows>
inline uint16_t CR::Core::Table<t_primaryKey, t_rows...>::GetIndex(const t_primaryKey& a_key) const {
	auto iter = m_lookUp.find(a_key);
	if(iter == m_lookUp.end()) {
		return c_unused;
	} else {
		return iter->second;
	}
}

template<typename t_primaryKey, typename... t_rows>
template<typename t_row>
inline const t_row& CR::Core::Table<t_primaryKey, t_rows...>::GetRow(uint16_t a_index) const {
	Log::Assert(a_index != c_unused && m_used[a_index], "asked for an unused row in table {}", m_tableName);

	return std::get<std::array<t_row, c_maxSize>>(m_rows)[a_index];
}

template<typename t_primaryKey, typename... t_rows>
template<typename t_row>
inline t_row& CR::Core::Table<t_primaryKey, t_rows...>::GetRow(uint16_t a_index) {
	Log::Assert(a_index != c_unused && m_used[a_index], "asked for an unused row in table {}", m_tableName);

	return std::get<std::array<t_row, c_maxSize>>(m_rows)[a_index];
}

template<typename t_primaryKey, typename... t_rows>
inline void CR::Core::Table<t_primaryKey, t_rows...>::DeleteRow(uint16_t a_index) {
	Log::Assert(a_index != c_unused && m_used[a_index], "tried to delete an unused row in table {}", m_tableName);

	m_used[a_index] = false;

	// To save on memory, if not a standard layout, then clear out the data, assuming if not standard layout
	// then a proper move assignment was written.
	if constexpr(!std::is_standard_layout_v<t_primaryKey>) { m_primaryKeys[a_index] = t_primaryKey{}; }
	ClearRows(a_index, std::index_sequence_for<t_rows...>{}, t_rows{}...);
}
