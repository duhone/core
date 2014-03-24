/*
similar to std::function
both hold a list of functions. MultiFunction will call every assigned function when invoked. 
SelectableFunction will invoke a single one of its functions which can be independently selected.
 */
#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include "Algorithm.h"

namespace CR
{
	namespace Core
	{
		template<typename FunctionT>
		class MultiFunction {};

		//no return type for multifunction, who would win?
		template<typename ReturnType, typename... ArgTypes>
		class MultiFunction<ReturnType (ArgTypes...)>
		{
			static_assert(std::is_same<ReturnType, void>::value,
				"MultiFunction only works with void return type");
		public:
			typedef std::function<ReturnType(ArgTypes...)> OperationType;

			std::size_t Size() const
			{
				return m_operations.size();
			}
			void operator=(std::nullptr_t)
			{
				m_operations.clear();
			}
			void operator=(OperationType operation)
			{
				*this = nullptr;
				*this += operation;
			}
			void operator+=(OperationType operation)
			{
				m_operations.push_back(operation);
			}
			template<typename... ArgTypes>
			void operator()(ArgTypes&&... params)
			{
				for (auto& op : m_operations)
				{
					op(std::forward<ArgTypes>(params)...);
				}
			}
			explicit operator bool() const
			{
				return AllOf(m_operations, [](const OperationType& op) -> bool {return static_cast<bool>(op); });
			}
		private:
			std::vector<OperationType> m_operations;
		};

		template<typename FunctionT>
		class SelectableFunction {};

		template<typename ReturnType, typename... ArgTypes>
		class SelectableFunction<ReturnType(ArgTypes...)>
		{
		public:
			typedef std::function<ReturnType(ArgTypes...)> OperationType;

			std::size_t Size() const
			{
				return m_operations.size();
			}
			void operator=(std::nullptr_t)
			{
				m_operations.clear();
			}
			void operator=(OperationType operation)
			{
				*this = nullptr;
				*this += operation;
			}
			void operator+=(OperationType operation)
			{
				m_operations.push_back(operation);
			}
			template<typename... ArgTypes>
			ReturnType operator()(ArgTypes&&... params)
			{
				return m_operations.at(m_currentOperation)(std::forward<ArgTypes>(params)...);
			}
			explicit operator bool() const
			{
				return static_cast<bool>(m_operations.at(m_currentOperation));
			}
			void SetOperation(std::size_t op) { m_currentOperation = op; }

			OperationType& operator[](std::size_t index)
			{
				return m_operations.at(index);
			}
		private:
			std::vector<OperationType> m_operations;
			std::size_t m_currentOperation{ 0 };
		};
	}
}
