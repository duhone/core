/*
similar to std::function
both hold a list of functions. MultiFunction will call every assigned function when invoked.
SelectableFunction will invoke a single one of its functions which can be independently selected.
*/
#pragma once

#include "core/Algorithm.h"
#include "core/Concepts.h"
#include <algorithm>
#include <functional>
#include <vector>

namespace CR::Core {
	template<Callable CallableT>
	class MultiFunction {};

	// no return type for multifunction, who would win?
	template<SemiRegular ReturnType, SemiRegular... ArgTypes>
	class MultiFunction<ReturnType(ArgTypes...)> final {
		static_assert(std::is_same_v<ReturnType, void>, "MultiFunction only works with void return type");

	  public:
		using OperationT = std::function<ReturnType(ArgTypes...)>;

		std::size_t size() const noexcept { return m_operations.size(); }

		void operator=(std::nullptr_t) { m_operations.clear(); }

		void operator=(const OperationT& a_operation) {
			*this = nullptr;
			*this += a_operation;
		}

		void operator+=(const OperationT& a_operation) { m_operations.push_back(a_operation); }

		template<SemiRegular... FArgTypes>
		void operator()(FArgTypes&&... a_params) {
			for(auto& op : m_operations) { op(std::forward<FArgTypes>(a_params)...); }
		}

		explicit operator bool() const {
			return all_of(m_operations, [](const OperationT& a_op) { return static_cast<bool>(a_op); });
		}

	  private:
		std::vector<OperationT> m_operations;
	};

	template<Callable CallableT>
	class SelectableFunction {};

	template<SemiRegular ReturnType, SemiRegular... ArgTypes>
	class SelectableFunction<ReturnType(ArgTypes...)> {
	  public:
		using OperationT = std::function<ReturnType(ArgTypes...)>;

		std::size_t size() const { return m_operations.size(); }

		void operator=(std::nullptr_t) { m_operations.clear(); }

		void operator=(std::size_t a_op) { SetOperation(a_op); }

		void operator=(const OperationT& a_operation) {
			*this = nullptr;
			*this += a_operation;
		}

		void operator+=(const OperationT& a_operation) { m_operations.push_back(a_operation); }

		template<SemiRegular... FArgTypes>
		ReturnType operator()(FArgTypes&&... a_params) {
			if(m_currentOperation >= 0 && m_currentOperation < m_operations.size()) {
				return m_operations.at(m_currentOperation)(std::forward<FArgTypes>(a_params)...);
			}
			return ReturnType{};
		}

		explicit operator bool() const { return static_cast<bool>(m_operations.at(m_currentOperation)); }

		void SetOperation(std::size_t a_op) { m_currentOperation = a_op; }

		OperationT& operator[](std::size_t a_index) { return m_operations.at(a_index); }

	  private:
		std::vector<OperationT> m_operations;
		std::size_t m_currentOperation{0};
	};
}    // namespace CR::Core
